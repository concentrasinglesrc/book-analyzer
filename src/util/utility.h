#ifndef UTILITY_H
#define UTILITY_H
#include <string>
#include <vector>

namespace utility {

inline std::vector<std::string> split_line(std::string const &line,
                                           char const delim = ' ') {
  std::vector<std::string> split;
  if (line.empty())
    return split;

  size_t pos = 0;
  do {
    size_t next = line.find(delim, pos);
    if (std::string::npos == next) {
      std::string sub = line.substr(pos);
      split.push_back(sub);
      pos = next;
    } else {
      std::string sub = line.substr(pos, next - pos);
      split.push_back(sub);
      pos = next + 1;
    }
  } while (std::string::npos != pos);
  return split;
}

} // namespace utility

#endif // !UTILITY_H
