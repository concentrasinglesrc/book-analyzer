#ifndef ORDER_DETAILS_H
#define ORDER_DETAILS_H
#include <cstdlib>
#include <memory>

namespace book {

class OrderDetails {

protected:
  long timestamp;
  std::string id;
  long shares;

public:
  OrderDetails(void) : timestamp(0l), id(""), shares(0l) {}

  OrderDetails(long const timestamp, std::string const id, long const shares)
      : timestamp(timestamp), id(id), shares(shares) {}

  OrderDetails(OrderDetails const &other)
      : timestamp(other.get_timestamp()), id(other.get_id()),
        shares(other.get_shares()) {}

  OrderDetails &operator=(OrderDetails const &other) {
    if (this == &other)
      return *this;
    timestamp = other.get_timestamp();
    id = other.get_id();
    shares = other.get_shares();
    return *this;
  }

  virtual ~OrderDetails(void) {}

  inline long get_timestamp(void) const { return timestamp; }
  inline std::string get_id(void) const { return id; }
  inline int get_shares(void) const { return shares; }
};

class AddOrderDetails : public OrderDetails {
  std::string side;
  float price;

public:
  AddOrderDetails(void) : OrderDetails(), side(""), price(0.0f) {}

  AddOrderDetails(long const timestamp, std::string const &id, int const shares,
                  std::string const &side, float const price)
      : OrderDetails(timestamp, id, shares), side(side), price(price) {}

  AddOrderDetails(AddOrderDetails const &other)
      : OrderDetails(other), side(other.get_side()), price(other.get_price()) {}

  AddOrderDetails &operator=(AddOrderDetails const &other) {
    if (this == &other)
      return *this;
    timestamp = other.get_timestamp();
    id = other.get_id();
    shares = other.get_shares();
    side = other.get_side();
    price = other.get_price();
    return *this;
  }

  inline std::string get_side(void) const { return side; }
  inline float get_price(void) const { return price; }
};

using ReduceOrderDetails =
    OrderDetails; // reduce order data is similar to parent type

} // namespace book

#endif // !ORDER_DETAILS_H