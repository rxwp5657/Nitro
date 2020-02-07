#include <context.hpp>

namespace nitro
{
    namespace graphics
    {
        Context* Context::inst_ = nullptr;
        bool Context::glew_ = false;
        
        Context::Context()
        {
            glfwInit();
    
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        }

        Context::~Context() noexcept
        {
            glfwTerminate();
        }
        
        Context* Context::get_instance()
        {
            if(inst_ == nullptr)
            {
                inst_ = new Context{};
            }

            return inst_;
        }

        void Context::init_glew()
        {
            if(!glew_)
            {
                glewExperimental = GL_TRUE;
                glewInit();
                glEnable(GL_DEPTH_TEST);
            }
        }
    }
}