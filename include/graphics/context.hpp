#ifndef CONTEXT_H
#define CONTEXT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace nitro
{
    namespace graphics
    {
        class Context
        {
        public:
        
            static Context* get_instance();
            static void init_glew();
            ~Context() noexcept;

        private:
            static Context* inst_;
            static bool glew_;
            Context();
            Context(const Context&);
            Context& operator=(const Context&);
        };
    }
}

#endif