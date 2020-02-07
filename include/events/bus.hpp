#ifndef BUS_H
#define BUS_H

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <event.hpp>
#include <command.hpp>

namespace nitro
{
    namespace events
    {   
        class EventBus
        {
        public:
            EventBus():subscribers_{}{}

            template <typename EvnT>
            void publish(EvnT& event)
            {   
                if(subscribers_.find(typeid(EvnT)) != subscribers_.end())
                {
                    auto handlers = subscribers_[typeid(EvnT)];

                    for(auto& handler : *handlers)
                        handler->execute(event);
                }
            }

            template<typename SubT, typename EvnT>
            void subscribe(SubT *subscriber, void (SubT::*memberF)(EvnT& event))
            {
                std::shared_ptr<Handlers>handlers{nullptr};

                if(subscribers_.find(typeid(EvnT)) == subscribers_.end())
                {
                    handlers = std::make_shared<Handlers>();
                    subscribers_[typeid(EvnT)] = handlers;
                }
                else
                {
                    handlers = subscribers_[typeid(EvnT)];
                }

                auto subscribers_p = std::make_shared<Command<SubT, EvnT>>(subscriber, memberF); 
                handlers->push_back(subscribers_p);
            }

        private:
            typedef std::vector<std::shared_ptr<CommandWrapper>> Handlers;
            std::map<std::type_index,std::shared_ptr<Handlers>> subscribers_;
        };
    }
}

#endif