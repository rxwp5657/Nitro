#ifndef COMMAND_H
#define COMMAND_H

#include <event.hpp>
#include <memory>

namespace nitro
{
    namespace events
    {
        struct Command
        {
        public:
            virtual ~Command() = default;
            virtual void call(Event& event) = 0;
        };

        template<typename SubT, typename EvnT>
        class CommandHandler : public Command
        {
        public:
            typedef void (SubT::*memberF)(const EvnT&);
            CommandHandler(SubT* subscriber, memberF event_function)
            : subscriber_{subscriber},
              function_{event_function}
            {}
            
            void call(Event& event) override
            {
                ((*subscriber_).*function_)(static_cast<EvnT&>(event));
            }

        private:
            SubT *subscriber_;
            memberF function_;
        };
    }
}

#endif