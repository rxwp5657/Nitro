#ifndef KEYBOARD_EVNT_H
#define KEYBOARD_EVNT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <event.hpp>

namespace nitro
{
    namespace input
    {
        struct KeyBoardEvent : events::Event 
        {
            KeyBoardEvent(GLenum key_pressed, float delta_time) 
            : key{key_pressed},
              time{delta_time}
            {}  
            
            GLenum key;
            float time;
        };
    }
}

#endif