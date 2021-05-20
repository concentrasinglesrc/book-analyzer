#ifndef ACTION_DETAILS_H
#define ACTION_DETAILS_H

namespace book {
class ActionDetails {

  long timestamp;
  float total;
  std::string side;
  long shares;

public:
  ActionDetails(long const timestamp, float const total,
                std::string const &side, long const shares)
      : timestamp(timestamp), total(total), side(side), shares(shares) {}

  ActionDetails(ActionDetails const &other)
      : timestamp(other.get_timestamp()), total(other.get_total()),
        side(other.get_side()), shares(other.get_shares()) {}

  ActionDetails &operator=(ActionDetails const &other) {
    if (this == &other)
      return *this;
    timestamp = other.get_timestamp();
    total = other.get_total();
    side = other.get_side();
    shares = other.get_shares();
  }

  inline long get_timestamp(void) const { return timestamp; }
  inline float get_total(void) const { return total; }
  inline std::string get_side(void) const { return side; }
  inline long get_shares(void) const { return shares; }
};

} // namespace book

#endif // !ACTION_DETAILS_H