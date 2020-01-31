#include <window.hpp>

namespace nitro
{
    namespace graphics
    {
        Window::Window(const unsigned short width, 
                       const unsigned short height, 
                       const std::string& title, 
                       const WindowType type,
                       bool resizable)
        : width_{width},
          height_{height},
          title_{title},
          type_{type}
        {
            if(resizable)
                glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
            else
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            if(type == WindowType::FULL_SCREEN)
                window_ptr_ = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
            else    
                window_ptr_ = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
            
            glfwMakeContextCurrent(window_ptr_);

            Context::init_glew();
        }

        Window::~Window() noexcept
        {
            glfwDestroyWindow(window_ptr_);
        }

        GLFWwindow* Window::get_window_ptr() const
        {
            return window_ptr_;
        }

        unsigned short Window::get_width()  const
        {
            return width_;
        }

        unsigned short Window::get_height() const
        {
            return height_;
        }

        WindowType Window::get_type() const
        {
            return type_;
        }
    }
}