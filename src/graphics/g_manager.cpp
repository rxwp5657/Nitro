#include <g_manager.hpp>

namespace nitro
{
    namespace graphics
    {
        Manager::Manager(const Window& window, const Shader& shader)
        : context_{Context::get_instance()},
          window_{window},
          active_program_{shader},
          shaders_{std::vector<Shader>{shader}}
        {
            
        }

        GLFWwindow* Manager::get_window() const
        {
            return window_.get_window_ptr();
        }
    }
    
}