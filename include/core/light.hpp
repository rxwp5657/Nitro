#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>
#include <math.h>
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

            void Range(float range);
            void Move(const clutch::Mat4<float>& transform);

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> color_;
            float max_distance_;
        };

        class SpotLight
        {
        public:

            SpotLight();

            SpotLight(clutch::Vec4<float> position,
                      clutch::Vec4<float> direction,
                      clutch::Vec4<float> color = {1.0f,1.0f,1.0f,1.0f},
                      float max_distance = 5.0f,
                      float umbra    = cos((clutch::PI * 17.5f) / 180.0f),
                      float penumbra = cos((clutch::PI * 12.5f) / 180.0f));

            void Umbra(float umbra);
            void Penumbra(float penumbra);

            void MovePos(const clutch::Mat4<float>& transform);
            void MoveDir(const clutch::Mat4<float>& transform);

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
            float max_distance_;
            float umbra_;
            float penumbra_;
        };

        class DirectionalLight
        {
        public:

            DirectionalLight();

            DirectionalLight(clutch::Vec4<float> direction,
                             clutch::Vec4<float> color);

            void Move(const clutch::Mat4<float>& transform);

        private:
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
        };
    }
}

#endif