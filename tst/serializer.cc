#include "serializers/serializer.h"
#include "mock.h"
#include "serializers/order-details.h"
#include "validators/input-validator.h"
#include <memory>
#include <string>
#include <vector>

class SerializerTest : public ::testing::Test {

protected:
  ::testing::NiceMock<mock::InputValidator> mock_validator;
  std::unique_ptr<book::Serializer> under_test;

public:
  void SetUp(void) { under_test.reset(new book::SerializerImpl(&mock_validator)); }
};

TEST_F(SerializerTest, given_deserialize_reduce_order_then_return_error_when_order_is_invalid) {
  std::vector<std::string> serialized {"28929980", "R", "ud", "60"};
  book::ReduceOrderDetails expected(28929980l, "ud", 60l);

  EXPECT_CALL(mock_validator, validate_reduce_order(::testing::_))
      .WillOnce(::testing::Return(false));

  book::ReduceOrderDetails actual;
  int rc = under_test->deserialize(actual, serialized);

  ASSERT_EQ(rc, -1);
}

TEST_F(SerializerTest, given_deserialize_reduce_order_then_succeed_when_order_is_valid) {
  std::vector<std::string> serialized {"28929980", "R", "ud", "60"};
  book::ReduceOrderDetails expected(28929980l, "ud", 60l);

  EXPECT_CALL(mock_validator, validate_reduce_order(::testing::_))
      .WillOnce(::testing::Return(true));

  book::ReduceOrderDetails actual;
  int rc = under_test->deserialize(actual, serialized);

  ASSERT_EQ(rc, 0);
  ASSERT_EQ(actual.get_id(), expected.get_id());
  ASSERT_EQ(actual.get_timestamp(), expected.get_timestamp());
  ASSERT_EQ(actual.get_shares(), expected.get_shares());
}

TEST_F(SerializerTest, given_deserialize_add_order_then_return_error_when_order_is_invalid) {
  std::vector<std::string> serialized {"28929980", "A", "ud", "S", "0.23", "60"};
  book::AddOrderDetails expected(28929980l, "ud", 60l, "S", 0.23f);

  EXPECT_CALL(mock_validator, validate_add_order(::testing::_))
      .WillOnce(::testing::Return(false));

  book::AddOrderDetails actual;
  int rc = under_test->deserialize(actual, serialized);

  ASSERT_EQ(rc, -1);
}

TEST_F(SerializerTest, given_deserialize_add_order_then_succeed_when_order_is_valid) {
  std::vector<std::string> serialized {"28929980", "A", "ud", "S", "0.23", "60"};
  book::AddOrderDetails expected(28929980l, "ud", 60l, "S", 0.23f);

  EXPECT_CALL(mock_validator, validate_add_order(::testing::_))
      .WillOnce(::testing::Return(true));

  book::AddOrderDetails actual;
  int rc = under_test->deserialize(actual, serialized);

  ASSERT_EQ(rc, 0);
  ASSERT_EQ(actual.get_id(), expected.get_id());
  ASSERT_EQ(actual.get_timestamp(), expected.get_timestamp());
  ASSERT_EQ(actual.get_shares(), expected.get_shares());
  ASSERT_EQ(actual.get_side(), expected.get_side());
  ASSERT_EQ(actual.get_price(), expected.get_price());
}

TEST_F(SerializerTest, given_serialize_then_return_total_when_target_is_lt_shares) {
  book::ActionDetails action_details(28929980l, 8000.00f, "S", 60l);
  std::string expected ="28929980 S 8000.00";

  auto actual = under_test->serialize(action_details, 30l);
  ASSERT_EQ(actual, expected);
}

TEST_F(SerializerTest, given_serialize_then_return_total_when_target_is_eq_shares) {
  book::ActionDetails action_details(28929980l, 8000.00f, "S", 60l);
  std::string expected ="28929980 S 8000.00";

  auto actual = under_test->serialize(action_details, 60l);
  ASSERT_EQ(actual, expected);
}

TEST_F(SerializerTest, given_serialize_then_return_na_total_when_target_is_gt_shares) {
  book::ActionDetails action_details(28929980l, 8000.00f, "S", 60l);
  std::string expected ="28929980 S NA";

  auto actual = under_test->serialize(action_details, 80l);
  ASSERT_EQ(actual, expected);
}

