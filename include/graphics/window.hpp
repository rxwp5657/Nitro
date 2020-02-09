#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <context.hpp>

namespace nitro 
{
    namespace graphics
    {
        enum class WindowType
        {
            WINDOWED,
            FULL_SCREEN
        };
    
        class Window
        {
        public:
            Window(const unsigned short width, 
                   const unsigned short height, 
                   const std::string& title, 
                   const WindowType type,
                   bool resizable);
            
            Window(const Window& other);
            Window& operator=(const Window& other);

            Window(Window&& other) noexcept;
            Window& operator=(Window&& other) noexcept;

            ~Window() noexcept;

            void Destroy() noexcept;

            GLFWwindow* get_window_ptr() const;
            unsigned short get_width()  const;
            unsigned short get_height() const;
            WindowType get_type() const;

        private:
            unsigned short width_;
            unsigned short height_;
            std::string title_; 
            WindowType type_;
            GLFWwindow* window_ptr_;
        };
    }
}

#endif