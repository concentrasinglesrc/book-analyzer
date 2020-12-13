#ifndef COMMAND_H
#define COMMAND_H
#include <string>
#include <vector>
#include "models/order.h"
#include "models/side.h"
#include "data/data-handler.h"
#include "serializers/serializer.h"
#include "viewers/viewer.h"

namespace book
{
    class InputValidator;

    class Command
    {
    protected:
        std::string name;
        DataHandler * handler;
        Serializer * serializer;
        Viewer * viewer;

    public:
        Command(std::string const && name, 
            DataHandler & handler,
            Serializer & serializer,
            Viewer & viewer)
        : name(name), handler(&handler), serializer(&serializer), 
            viewer(&viewer) {}

        virtual ~Command(void) {} 
        virtual int exec(std::vector<std::string> const & args,
            long const target_shares) = 0;

        inline std::string get_name(void) const { return name; }

        void update(models::Order const & order, long const target_shares) {
            models::Side old_side = handler->get_side(order.get_side());

            handler->update_order(order);
            handler->update_side(order.get_timestamp(), order.get_side(), 
                target_shares);

            models::Side new_side = handler->get_side(order.get_side());

            if (old_side.get_total() != new_side.get_total()) {
                viewer->render(new_side, target_shares);
            } 

        }

        template<class T>
        static std::unique_ptr<Command> make_command(DataHandler & handler, Serializer & serializer, Viewer & viewer) {
            return std::make_unique<T>(handler, serializer, viewer);
        }

    };
    
} // namespace book

#endif // !COMMAND_H
