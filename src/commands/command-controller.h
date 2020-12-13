#ifndef COMMAND_CONTROLLER_H
#define COMMAND_CONTROLLER_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "command.h"
#include "data/data-handler.h"
#include "logger/log.h"
#include "models/order.h"
#include "validators/input-validator.h"
#include "viewers/viewer.h"

namespace book
{
    class AddOrderCommand : public Command
    {
        static logger::Log log;
    public:
        AddOrderCommand(DataHandler & handler, Serializer & serializer, Viewer & viewer)
        : Command("AddOrder", handler, serializer, viewer) {}

        ~AddOrderCommand(void) {}
        int exec(std::vector<std::string> const & args, long const target_shares) {
            AddOrderDetails order;
            if (0 < serializer->deserialize(order, args)) {
              log.warn("Serializer error"); 
              return -1;
            }
            return add(order, target_shares);
        }

        int add(AddOrderDetails const order_details, long const target_shares) {
            log.debug("AddOrder {id : %s, timestamp: %ld, side: %s, price: %.2f, shares: %d}", 
                order_details.get_id().c_str(), 
                order_details.get_timestamp(),
                order_details.get_side().c_str(),
                order_details.get_price(),
                order_details.get_shares());

            models::Order new_order = models::Order::builder()
                .with_id(order_details.get_id())
                .with_timestamp(order_details.get_timestamp())
                .with_shares(order_details.get_shares())
                .with_side(order_details.get_side())
                .with_price(order_details.get_price())
                .build();


            update(new_order, target_shares);
            return 0;
        }

    };

    class ReduceOrderCommand : public Command
    {
        static logger::Log log;
    public:
        ReduceOrderCommand(DataHandler & handler, Serializer & serializer, Viewer & viewer)
        : Command("ReduceOrder", handler, serializer, viewer){}
        ~ReduceOrderCommand(void) {}

        int exec(std::vector<std::string> const & args, long const target_shares) {
            ReduceOrderDetails order;
            if(0 < serializer->deserialize(order, args)) {
              log.warn("Serializer error");
              return -1;
            }
            return reduce(order, target_shares);
        }

        int reduce(ReduceOrderDetails const order_details, long const target_shares) {
            log.debug("ReduceOrder {id : %s, timestamp %ld, shares: %d}", 
                order_details.get_id().c_str(), 
                order_details.get_timestamp(),
                order_details.get_shares());
            auto order_model = handler->get_order(order_details.get_id());
            auto empty_order = models::Order::builder().build();

            if (empty_order.get_timestamp() == order_model.get_timestamp()) {
                log.warn("Unable to process ReduceOrderCommand, order id %s"
                    "does not exist", order_details.get_id()); 
                return -1;
            }

            order_model.set_timestamp(order_details.get_timestamp());
            order_model.set_side(order_model.get_side());
            order_model.set_shares(order_model.get_shares() - order_details.get_shares());

            update(order_model, target_shares);
            return 0;
        }
    };
    
    class CommandController
    {
    private:
        std::unordered_map<std::string,
            std::function<std::unique_ptr<Command> (DataHandler&, Serializer&, Viewer&)> >
            dispatcher;

    public:
        CommandController() : dispatcher() {}
        virtual ~CommandController() {}

        void subscribe(std::string const && command, 
            std::function<std::unique_ptr<Command> (DataHandler&, 
                Serializer&, Viewer&)> make_command) {
            dispatcher[command] = make_command;
        }

        std::unique_ptr<Command> parse_command(std::string const & command, 
            Serializer & serializer, 
            DataHandler & handler,
            Viewer & viewer) {
            auto it = dispatcher.find(command);
            if (dispatcher.end() == it) return std::unique_ptr<Command>();
            else return it->second(handler, serializer, viewer);
        }

    };

    logger::Log AddOrderCommand::log(__FILE__ "-AddOrderdCommand", logger::INFO);
    logger::Log ReduceOrderCommand::log(__FILE__ "-ReduceOrderdCommand", logger::INFO);
    
} // namespace book

#endif // !COMMAND_CONTROLLER_H

