#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <event.hpp>
#include <memory>

namespace nitro
{
    namespace events
    {
        struct CommandWrapper
        {
        public:
            virtual ~CommandWrapper() = default;
            void execute(Event& event)
            {
                call(event);
            }
        private:
            virtual void call(Event& event) = 0;
        };

        template<typename SubT, typename EvnT>
        class Command : public CommandWrapper
        {
        public:
            typedef void (SubT::*memberF)(const EvnT&);
            Handler(SubT* subscriber, memberF event_function)
            : subscriber_{subscriber},
              function_{event_function}
            {}
            
            void call(Event& event) override
            {
                ((*subscriber_).*function_)(static_cast<EvnT&>(event));
            }

        private:
            SubT   *subscriber_;
            memberF function_;
        };
    }
}

#endif