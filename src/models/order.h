#ifndef ORDER_H
#define ORDER_H

#include <string>

namespace book {
namespace models {

class Order {
  std::string id;
  long timestamp;
  long shares;
  float price;
  std::string side;

public:
  class Builder {
    std::string id;
    long timestamp;
    long shares;
    float price;
    std::string side;

  public:
    Builder(void) : id(""), timestamp(0l), shares(0l), price(0.0f), side("") {}

    Order build(void) { return Order(id, timestamp, shares, price, side); }

    inline Builder &with_id(std::string const &id) {
      this->id = id;
      return *this;
    }

    inline Builder &with_timestamp(long const timestamp) {
      this->timestamp = timestamp;
      return *this;
    }

    inline Builder &with_shares(long const shares) {
      this->shares = shares;
      return *this;
    }

    inline Builder &with_price(float const price) {
      this->price = price;
      return *this;
    }

    inline Builder &with_side(std::string const &side) {
      this->side = side;
      return *this;
    }
  };

  static Builder builder(void) { return Builder(); }

  Order(void) : id(""), timestamp(0l), shares(0l), price(0.0f), side("") {}

  Order(std::string const &id, long const timestamp, long const shares,
        float const price, std::string const &side)
      : id(id), timestamp(timestamp), shares(shares), price(price), side(side) {
  }

  Order(Order const &other)
      : id(other.get_id()), timestamp(other.get_timestamp()),
        shares(other.get_shares()), price(other.get_price()),
        side(other.get_side()) {}

  Order &operator=(Order const &order) {
    if (this == &order)
      return *this;
    this->id = order.get_id();
    this->timestamp = order.get_timestamp();
    this->shares = order.get_shares();
    this->price = order.get_price();
    this->side = order.get_side();
    return *this;
  }

  inline std::string get_id(void) const { return id; }
  inline long get_timestamp(void) const { return timestamp; }
  inline long get_shares(void) const { return shares; }
  inline float get_price(void) const { return price; }
  inline std::string get_side(void) const { return side; }

  inline void set_id(std::string const &id) { this->id = id; }
  inline void set_timestamp(long const timestamp) {
    this->timestamp = timestamp;
  }
  inline void set_shares(long const shares) { this->shares = shares; }
  inline void set_price(float const price) { this->price = price; }
  inline void set_side(std::string const &side) { this->side = side; }
};

inline bool operator==(Order const &o1, Order const &o2) {
  return o1.get_timestamp() == o2.get_timestamp() &&
         o1.get_id() == o2.get_id() && o1.get_shares() == o2.get_shares() &&
         o1.get_side() == o2.get_side();
}

inline bool operator!=(Order const &o1, Order const &o2) { return !(o1 == o2); }

} // namespace models
} // namespace book

#endif // !ORDER_H