#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <memory>
#include <initializer_list>
#include <window.hpp>
#include <command.hpp>
#include <keyboard_evnt.hpp>

namespace nitro
{
    namespace input
    {
        class Controller
        {
        public:
            Controller();
            void HandleInput(const graphics::Window& window, float time);

            template<typename T, typename F>
            void AddButton(GLenum trigger,T *obj, void (T::*memberF)(const F& function))
            {
                if(buttons_.find(trigger) == buttons_.end())
                {
                    buttons_[trigger] = std::make_shared<events::CommandHandler<T,F>>(obj, memberF);
                }
            }

        private:
            std::map<GLenum, std::shared_ptr<events::Command>> buttons_; 
        };
    }
}

#endif