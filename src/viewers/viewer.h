#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>
#include "data/data-handler.h"
#include "fruit/fruit.h"
#include "logger/log.h"
#include "models/side.h"
#include "serializers/serializer.h"

namespace book
{

    class Viewer {
      public:

      virtual void render(models::Side const & side, long const target_shares) = 0;
      virtual std::string order_to_action_side(std::string const & order_side) = 0;
    };

    class ViewerImpl : public Viewer
    {
        std::ostream * os;
        Serializer * serializer;
        static logger::Log log;
    public:
        INJECT(ViewerImpl(std::ostream * os, Serializer * serializer)) : os (os), serializer(serializer) {}
        virtual ~ViewerImpl(void) {}

        void render(models::Side const & side, long const target_shares) {
            ActionDetails action_details = 
                ActionDetails(side.get_timestamp(), 
                    side.get_total(),
                    order_to_action_side(side.get_side()),
                    side.get_shares());

            std::string output = serializer->serialize(action_details, target_shares);

            if (!output.empty()){
                (*os) << output << std::endl;
            }
        }

        /**
         * Order side is B for bidorders and S for askorders
         * Action side is S for sell to bidorders and B is for buy from askorders
         */
        std::string order_to_action_side(std::string const & order_side) {
            if ("S" == order_side) {
                return "B";
            } else if ("B" == order_side) {
                return "S";
            } else {
                return order_side;
            }
        }
    };

    logger::Log ViewerImpl::log(__FILE__);
    
} // namespace book

#endif // !VIEWER_H
