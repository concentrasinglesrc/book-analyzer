#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H
#include <string>
#include <unordered_map>
#include <vector>

#include "logger/log.h"
#include "models/side-order.h"

namespace book
{
    class DataHandler
    {
        private:

        public:
            virtual ~DataHandler(void) {}

            virtual models::Side get_side(std::string const & side) = 0;
            virtual models::Order get_order(std::string const & id) = 0;
            virtual void update_order(models::Order const & order) = 0;
            virtual void update_side(long const timestamp, std::string const & side, long const target_shares) = 0;
    };

    class LocalDataHandler : public DataHandler
    {
    private:
        models::SideOrder buy_expenses;
        models::SideOrder sell_income;
        static logger::Log log;
    public:
        LocalDataHandler(void) 
        : buy_expenses("B"), sell_income("S") {}
        ~LocalDataHandler(void) {}

        models::Side get_side(std::string const & side) {
            auto side_order = choose_side_order(side);
            return side_order.get_side();
        }

        models::Order get_order(std::string const & id) {
            auto & bidorders = buy_expenses.get_orders();
            auto bidorder = bidorders.find(id);
            if (bidorders.end() != bidorder) return bidorder->second;
            
            auto & askorders = sell_income.get_orders();
            auto  askorder = askorders.find(id);
            if (askorders.end() != askorder) return askorder->second;

            return models::Order();
        }

        void update_order(models::Order const & order) {
            if (!validate(order)) return;
            log.debug("update %s-%s-%.2f order", order.get_id().c_str(), 
                order.get_side().c_str(), order.get_price());
            auto & side_order = choose_side_order(order.get_side());
            auto & orders = side_order.get_orders();
            orders[order.get_id()] = order;

            auto & sorted_orders = side_order.get_orders_by_price();
            sorted_orders[order.get_price()].insert(order.get_id());

            log.apply_if(logger::INFO, [this] () {
                this->dump_orders(sell_income.get_orders());
                this->dump_orders(buy_expenses.get_orders());
            });
        }

        void update_side(long const timestamp, std::string const & side, long const target_shares) {
            if (side == buy_expenses.get_side().get_side()) {
                auto & sorted_orders = buy_expenses.get_orders_by_price();
                // calculate with highest bids to buy
                update_total(buy_expenses.get_side(), sorted_orders.rbegin(), 
                    sorted_orders.rend(),
                    timestamp,
                    target_shares);
            } else {
                // calculate with lowest asks to sell 
                auto & sorted_orders = sell_income.get_orders_by_price();
                update_total(sell_income.get_side(), sorted_orders.begin(),
                    sorted_orders.end(),
                    timestamp,
                    target_shares);
            }
        }

    private:
        template<typename Container>
        void dump_orders(Container const & orders) {
            log.info("{");
            for (auto & p : orders) {
                log.info("  { id: %s, order: { side: %s, timestamp: %ld, "
                    "price: %.2f, shares: %ld}},", p.first.c_str(), 
                    p.second.get_side().c_str(), 
                    p.second.get_timestamp(),
                    p.second.get_price(),
                    p.second.get_shares());
            }
            log.info("}");
        }

        template<typename Iterator>
        void update_total(models::Side & side, Iterator begin, Iterator end, 
            long const timestamp,
            long const target_shares) {

            float total = 0.0f;
            long shares = 0;
            long required_shares = target_shares;
            for (auto it = begin; it != end && target_shares > 0; ++it) {
                log.debug("price: %.2f", it->first);
                for (auto & id : it->second) {
                    auto order = get_order(id);
                    if (required_shares > order.get_shares()) {
                        required_shares -= order.get_shares();
                        shares += order.get_shares();
                        total += order.get_price() * order.get_shares();
                    } else {
                        total  += order.get_price() * required_shares;
                        shares += required_shares;
                        required_shares -= order.get_shares();
                        break;
                    }
                }
            }

            log.info("Calculated %s total: %.2f, shares: %ld, remainder: %ld", side.get_side().c_str(),
                total, 
                shares,
                target_shares); 

            if (target_shares > shares) {
                total = 0.0f;
                shares = 0l;
            }

            log.info("Store %s total: %.2f, shares: %ld", side.get_side().c_str(),
                total, 
                shares); 
            side.set_timestamp(timestamp);
            side.set_total(total);
            side.set_shares(shares);
        }

        models::SideOrder & choose_side_order(std::string const & side) {
            if (side == sell_income.get_side().get_side()) return sell_income;
            return buy_expenses;
        }

        bool validate(models::Order const & order) {
            if (order.get_shares() < 0) {
                log.warn("Unable to process Order, shares is negative %d", order.get_shares());
                return false;
            }

            return true;
        }
    };

    logger::Log LocalDataHandler::log(__FILE__);
    
} // namespace book

#endif // !DATA_HANDLER_H
