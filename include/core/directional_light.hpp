#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include <vec4.hpp>
#include <transforms.hpp>

namespace nitro 
{
    namespace core
    {
        class DirectionalLight
        {
        public:
            DirectionalLight();
            DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color);
            void Move(float x, float y, float z);

        private:
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
        };
    }
}


#endif