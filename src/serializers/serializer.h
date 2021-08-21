#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "action-details.h"
#include "fruit/fruit.h"
#include "logger/log.h"
#include "order-details.h"
#include "util/utility.h"
#include "validators/input-validator.h"
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace book {
class Serializer {
public:
  virtual int deserialize(ReduceOrderDetails &order,
                          std::vector<std::string> const &args) = 0;
  virtual int deserialize(AddOrderDetails &order,
                          std::vector<std::string> const &args) = 0;
  virtual std::string serialize(ActionDetails const &action,
                                long const target_shares) = 0;
};

class SerializerImpl : public Serializer {
  InputValidator *validator;
  static logger::Log log;

public:
  INJECT(SerializerImpl(InputValidator *validator)) : validator(validator) {}

  virtual ~SerializerImpl(void) {}

  int deserialize(ReduceOrderDetails &order,
                  std::vector<std::string> const &args) {
    if (!validator->validate_reduce_order(args))
      return -1;

    long const timestamp = std::atol(args[0].c_str());
    std::string const id = args[2];
    long const size = std::atol(args[3].c_str());

    order = OrderDetails(timestamp, id, size);
    return 0;
  }

  int deserialize(AddOrderDetails &order,
                  std::vector<std::string> const &args) {
    if (!validator->validate_add_order(args))
      return -1;

    long const timestamp = std::atol(args[0].c_str());
    std::string const id = args[2];
    std::string const side = args[3];
    float const price = std::atof(args[4].c_str());
    long const size = std::atol(args[5].c_str());

    order = AddOrderDetails(timestamp, id, size, side, price);
    return 0;
  }

  std::string serialize(ActionDetails const &action, long const target_shares) {
    std::stringstream ss;
    ss << action.get_timestamp() << " " << action.get_side() << " ";

    if (target_shares > action.get_shares()) {
      ss << "NA";
    } else {
      ss << std::setprecision(2) << std::fixed << action.get_total();
    }

    return ss.str();
  }
};

logger::Log SerializerImpl::log(__FILE__);
} // namespace book

#endif // !SERIALIZER_H
