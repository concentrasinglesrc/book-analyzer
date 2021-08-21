#include "validators/input-validator.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

enum ValidatorType {
  ORDER_VALIDATOR,
  REDUCE_ORDER_VALIDATOR,
  ADD_ORDER_VALIDATOR
};

struct ValidatorData {
  std::vector<std::string> const args;
  bool ret_val;
  enum ValidatorType type;
};

class InputValidatorTest : public ::testing::TestWithParam<ValidatorData> {

protected:
  book::InputValidatorImpl under_test;
};

TEST_P(InputValidatorTest, given_validate_order_parameterized) {
  bool actual;

  switch (GetParam().type) {
  case ORDER_VALIDATOR:
    actual = under_test.validate_order(GetParam().args);
    break;
  case REDUCE_ORDER_VALIDATOR:
    actual = under_test.validate_reduce_order(GetParam().args);
    break;
  case ADD_ORDER_VALIDATOR:
    actual = under_test.validate_add_order(GetParam().args);
    break;
  default:
    FAIL();
  };

  ASSERT_EQ(actual, GetParam().ret_val);
}

static struct ValidatorData OrderDetailsData[] = {

    {{}, false, ORDER_VALIDATOR},               // failure on 0 args
    {{"", "", "", ""}, false, ORDER_VALIDATOR}, // failure on empty args
    {{"-5", "A", "ye", "B", "43.88", "100"},
     false,
     ORDER_VALIDATOR}, // failure timestamp num
    {{"29024925", "X", "ye", "B", "43.88"},
     false,
     ORDER_VALIDATOR}, // failure num of args
    {{"es", "X", "ye", "B", "43.88"},
     false,
     ORDER_VALIDATOR}, // failure timestamp is not num
    {{"29024925", "X", "ye", "B", "43.88", "100"},
     false,
     ORDER_VALIDATOR}, // failure order type
    {{"29024925", "R", "xe", "100"},
     true,
     ORDER_VALIDATOR}, // successfull reduce order
    {{"29024925", "A", "ye", "B", "43.88", "100"},
     true,
     ORDER_VALIDATOR}, // successfull add order
};

INSTANTIATE_TEST_SUITE_P(OrderInputValidator, InputValidatorTest,
                         testing::ValuesIn(OrderDetailsData));

static struct ValidatorData ReduceOrderDetailsData[] = {
    {{}, false, REDUCE_ORDER_VALIDATOR},               // failure on empty
    {{"", "", "", ""}, false, REDUCE_ORDER_VALIDATOR}, // failure on empty args
    {{"-5", "R", "ue", "100"},
     false,
     REDUCE_ORDER_VALIDATOR}, // failure negative timestamp
    {{"ex", "R", "ue", "100"},
     false,
     REDUCE_ORDER_VALIDATOR}, // failure timestamp is not a num
    {{"29024925", "A", "ye", "B", "43.88", "100"},
     false,
     REDUCE_ORDER_VALIDATOR}, // failure order type is not reduce order
    {{"29019435", "R", "ue", "-100"},
     false,
     REDUCE_ORDER_VALIDATOR}, // failure negative size
    {{"29019435", "R", "ue", "100"},
     true,
     REDUCE_ORDER_VALIDATOR}, // successfull reduce order
};

INSTANTIATE_TEST_SUITE_P(ReduceOrderInputValidator, InputValidatorTest,
                         testing::ValuesIn(ReduceOrderDetailsData));

static struct ValidatorData AddOrderDetailsData[] = {
    {{}, false, ADD_ORDER_VALIDATOR}, // failure on empty
    {{"-5", "A", "ye", "B", "43.88", "100"},
     false,
     ADD_ORDER_VALIDATOR}, // failure negative num
    {{"29024925", "A", "ye", "B", "43.88", "100"},
     true,
     ADD_ORDER_VALIDATOR}, // successfull
};

INSTANTIATE_TEST_SUITE_P(AddOrderInputValidator, InputValidatorTest,
                         testing::ValuesIn(AddOrderDetailsData));
