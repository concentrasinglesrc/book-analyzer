#ifndef SIDE_ORDER_H
#define SIDE_ORDER_H

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "order.h"
#include "side.h"

namespace book
{
    namespace models
    {
        class SideOrder {

            models::Side side;
            std::unordered_map<std::string, models::Order> orders;
            std::map<float, std::unordered_set<std::string>> sorted_orders;

            public:
            explicit SideOrder(std::string const & side) 
            : side(0, 0.0f, side, 0l), orders() {}

            inline models::Side & get_side(void) {return side;}
            inline std::unordered_map<std::string, models::Order> & get_orders(void) {
                return orders;
            }

            inline std::map<float, std::unordered_set<std::string>> & get_orders_by_price(void) {
                return sorted_orders;
            }

        };
    } // namespace models
    
    
} // namespace book



#endif // !SIDE_ORDER_H
