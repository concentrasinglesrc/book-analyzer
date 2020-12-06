#ifndef SIDE_H
#define SIDE_H

#include <string>

namespace book
{
namespace models
{
    
    class Side {
        long timestamp;
        float total;
        std::string side;
        long shares;

        public:
        Side(void)
        : timestamp(0l), total(0.0), side(""), shares(0l) {}

        Side(long const timestamp, float const total, std::string const & side, long const shares)
        : timestamp(timestamp), total(total), side(side), shares(shares) {}

        Side(Side const & other)
        : timestamp(other.get_timestamp()), total(other.get_total()), 
            side(other.get_side()),
            shares(other.get_shares()) {}

        Side & operator=(Side const & other) {
            if (this == &other) return *this;
            this->timestamp = other.get_timestamp();
            this->total = other.get_total();
            this->side = other.get_side();
            this->shares = other.get_shares();
            return *this;
        }
        
        inline long get_timestamp(void) const {return timestamp;}
        inline float get_total(void) const {return total;}
        inline std::string get_side(void) const {return side;}
        inline long get_shares(void) const {return shares;}

        inline void set_timestamp(long const timestamp) { this->timestamp = timestamp;}
        inline void set_total(float const total) {this->total = total;}
        inline void set_side(std::string const & side) {this->side = side;}
        inline void set_shares(long const shares) { this->shares = shares;}
    };

} // namespace models
} // namespace book


#endif // !SIDE_H
