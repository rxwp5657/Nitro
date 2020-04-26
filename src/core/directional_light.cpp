#include <directional_light.hpp>

namespace nitro
{
    namespace core
    {
        DirectionalLight::DirectionalLight()
          // Directions (vectors) actually have w = 0. However, if we want to translate
          // the light using a matrix tranform we need to set the w component
          // to w = 1 otherwise, the translation won't have any effect; 
        : direction_{0.0f,0.0f,0.0f,1.0f},
          color_{1.0f,1.0f,1.0f,1.0f}
        {

        }

        DirectionalLight::DirectionalLight(clutch::Vec4<float> direction, clutch::Vec4<float> color)
        : direction_{direction},
          color_{color}
        {

        }

        void DirectionalLight::Move(float x, float y, float z)
        {
            direction_ = clutch::Translation(x,y,z) * direction_;
        }
    }
}
