#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include "fruit/fruit.h"
#include "logger/log.h"
#include <iostream>
#include <string>
#include <vector>

#define VALIDATE_NUM(var, arg)                                                 \
  if (!parse_num(var, arg)) {                                                  \
    log.warn("cannot parse %d", var);                                          \
    return false;                                                              \
  }

namespace book {

class InputValidator {
public:
  virtual bool validate_order(std::vector<std::string> const &args) = 0;
  virtual bool validate_reduce_order(std::vector<std::string> const &args) = 0;
  virtual bool validate_add_order(std::vector<std::string> const &args) = 0;
};

class InputValidatorImpl : public InputValidator {
  static long const DAY_IN_MS = 86400000l;
  static logger::Log log;

public:
  INJECT(InputValidatorImpl(void)) = default;

  virtual ~InputValidatorImpl(void) {}

  bool validate_order(std::vector<std::string> const &args) {
    if (!validate_empty(args))
      return false;

    if (args.size() != 6 && args.size() != 4) {
      log.warn("malformed order, arg size mismatch");
      return false;
    }

    long timestamp;
    VALIDATE_NUM(timestamp, args[0]);
    log.info("timestamp = %d", timestamp);

    if (timestamp < 0 || timestamp > DAY_IN_MS) {
      log.warn("timestamp out of day bounds");
      return false;
    }

    if (args[1] != "A" && args[1] != "R") {
      log.warn("order types not supported %s", args[1].c_str());
      return false;
    }
    return true;
  }

  bool validate_reduce_order(std::vector<std::string> const &args) {
    if (!validate_order(args)) return false;

    if (args.size() != 4) {
      log.warn("malformed reduce order, arg size mismatch");
      return false;
    }

    long size;
    VALIDATE_NUM(size, args[3]);

    if (size < 0) {
      log.warn("malformed reduce order, size is out of bound");
      return false;
    }
    return true;
  }

  bool validate_add_order(std::vector<std::string> const &args) {
    if (!validate_order(args)) return false;

    if (args.size() != 6) {
      log.warn("malformed add order, arg size mismatch");
      return false;
    }

    if (args[3] != "S" && args[3] != "B") {
      log.warn("side is not supported %s", args[3].c_str());
      return false;
    }

    float price;
    VALIDATE_NUM(price, args[4]);

    if (price < 0.0f) {
      log.warn("[WARN]: malformed add order, price can be only positive");
      return false;
    }

    long size;
    VALIDATE_NUM(size, args[5]);

    if (size < 0) {
      log.warn("malformed add order, size is out of bound");
      return false;
    }

    return true;
  }

private:
  bool parse_num(long &l, std::string const &arg) {
    try {
      l = std::stol(arg);
      return true;
    } catch (std::invalid_argument const &e) {
      return false;
    }
  }

  bool parse_num(float &f, std::string const &arg) {
    try {
      f = std::stof(arg);
      return true;
    } catch (std::invalid_argument const &e) {
      return false;
    }
  }

  bool validate_empty(std::vector<std::string> const &args) {
    if (args.empty()) {
      log.warn("empty line");
      return false;
    }

    for (auto &it : args) {
      if (it.empty()) {
        log.warn("some args are empty in line");
        return false;
      }
    }

    return true;
  }
};

} // namespace book

#endif // !INPUT_VALIDATOR_H
