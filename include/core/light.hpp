#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>
#include <vec3.hpp>
#include <mat4.hpp>
#include <model.hpp>
#include <drawable.hpp>
#include <shader.hpp>
#include <transforms.hpp>

namespace nitro
{
    namespace core
    {
        class PointLight
        {
        public:
            PointLight(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& color,
                       const float max_distance);
            
            PointLight();

            void Move(const clutch::Mat4<float>& transform);

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> color_;
            float max_distance_;
        };
    }
}

#endif