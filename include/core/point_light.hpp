#ifndef POINT_LIGHT
#define POINT_LIGHT

#include <vec4.hpp>
#include <transforms.hpp>

namespace nitro 
{
    namespace core
    {
        class PointLight
        {
        public:

            PointLight();
            PointLight(const clutch::Vec4<float>& position, 
                       const clutch::Vec4<float>& color,
                       const float max_distance);

            void Range(float range);
            void Move(float x, float y, float z);

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> color_;
            float max_distance_;
        };
    }
}


#endif