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
            glfwSetInputMode(window_ptr_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            Context::init_glew();
        }

        Window::Window(const Window& other)
        : width_{other.width_},
          height_{other.height_},
          title_{other.title_},
          type_{other.type_},
          window_ptr_{other.window_ptr_}
        {

        }

        Window& Window::operator=(const Window& other)
        {
            width_  = other.width_;
            height_ = other.height_;
            title_  = other.title_;
            type_   = other.type_;
            window_ptr_ = other.window_ptr_;
            return *this;
        }

        Window::Window(Window&& other) noexcept
        : width_{other.width_},
          height_{other.height_},
          title_{other.title_},
          type_{other.type_},
          window_ptr_{other.window_ptr_}
        {
            other.width_  = 0;
            other.height_ = 0;
            other.title_  = "";
            other.window_ptr_ = nullptr; 
        }

        Window& Window::operator=(Window&& other) noexcept
        {
            if(this == &other) return *this;
            width_  = other.width_;
            height_ = other.height_;
            title_  = other.title_;
            type_   = other.type_;
            window_ptr_ = other.window_ptr_;
            other.width_  = 0;
            other.height_ = 0;
            other.title_  = "";
            other.window_ptr_ = nullptr; 

            return *this;
        }

        Window::~Window() noexcept
        {
        }

        void Window::Destroy() noexcept
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