#ifndef MOCK_H
#define MOCK_H
#include "validators/input-validator.h"
#include "gmock/gmock.h"

namespace mock {

class InputValidator : public book::InputValidator {
public:
  MOCK_METHOD1(validate_order, bool(std::vector<std::string> const &args));
  MOCK_METHOD1(validate_reduce_order,
               bool(std::vector<std::string> const &args));
  MOCK_METHOD1(validate_add_order, bool(std::vector<std::string> const &args));
};

} // namespace mock

#endif // MOCK_H
