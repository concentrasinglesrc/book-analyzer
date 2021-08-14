#include "log.h"

using namespace logger;

FILE *Log::os_map[] = {stderr, stderr, stdout, stdout};
char const *Log::sev_prefix[] = {"ERROR", "WARN", "INFO", "DEBUG"};
enum Sev Log::default_max_sev;

