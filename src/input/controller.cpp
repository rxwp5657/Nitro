#include <controller.hpp>

namespace nitro
{
    namespace input
    {
        Controller::Controller()
        : buttons_{}
        {

        }

        void Controller::HandleInput(const graphics::Window& window, float time)
        {
            glfwPollEvents();
            for(auto& button : buttons_)
            {
                if(glfwGetKey(window.get_window_ptr(), button.first) == GLFW_PRESS)
                {
                    auto event = KeyBoardEvent{button.first, time};
                    (button.second)->call(event);
                }
            }
        }
    }
}