#ifndef LOGGER_H
#define LOGGER_H
#include <cstdarg>
#include <functional>
#include <iostream>
#include <string>

#define debug(...) log(logger::DEBUG, __LINE__, __VA_ARGS__);
#define info(...) log(logger::INFO, __LINE__, __VA_ARGS__);
#define warn(...) log(logger::WARN, __LINE__, __VA_ARGS__);
#define error(...) log(logger::ERROR, __LINE__, __VA_ARGS__);

namespace logger {
enum Sev { ERROR = 0, WARN, INFO, DEBUG };

class Log {

  std::string facility;
  enum Sev max_sev;
  bool overriden;
  static FILE *os_map[];
  static char const *sev_prefix[];
  static enum Sev default_max_sev;

public:
  Log(std::string const &facility, enum Sev const max_sev)
      : facility(facility), max_sev(max_sev), overriden(true) {}

  explicit Log(std::string const &facility)
      : facility(facility), max_sev(default_max_sev), overriden(false) {}

  static void set_default_max_sev(enum Sev const max_sev) {
    default_max_sev = max_sev;
  }

  void log(enum Sev const sev, int const line, char const *fmt, ...) {
    if (!validate_severity(sev))
      return;

    FILE *stream = os_map[sev];
    va_list vl;
    fprintf(stream, "%s:%d [%s]: ", facility.c_str(), line, sev_prefix[sev]);
    va_start(vl, fmt);
    vfprintf(stream, fmt, vl);
    va_end(vl);
    fprintf(stream, "\n");

    if (ERROR == sev)
      exit(-1);
  }

  void apply_if(logger::Sev const sev, std::function<void(void)> lmb) {
    if (!validate_severity(sev))
      return;
    lmb();
  }

  inline logger::Sev get_max_sev(void) const { return max_sev; }

private:
  inline bool validate_severity(enum Sev const sev) {
    if (overriden)
      return sev <= max_sev;
    else
      return sev <= default_max_sev;
  }
};

} // namespace logger

#endif // !LOGGER_H
