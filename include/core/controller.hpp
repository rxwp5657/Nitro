#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <command.hpp>

namespace nitro
{
    namespace core
    {
        class Controller
        {
        public:
            Controller();
        private:
            std::map<GLenum, events::CommandWrapper> buttons_;
        };
    }
}

#endif