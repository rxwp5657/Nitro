#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>
#include <vec3.hpp>
#include <model.hpp>
#include <actor.hpp>
#include <shader.hpp>
#include <transforms.hpp>

namespace nitro
{
    namespace core
    {
        class PointLight : public Actor
        {
        public:
            PointLight(const std::string& model,
                       const clutch::Vec3<float>& position, 
                       const clutch::Vec3<float>& color,
                       const float max_distance);
            
            PointLight();

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader)  override;

        private:
            clutch::Vec3<float> position_;
            clutch::Vec3<float> color_;
            float max_distance_;
        };
    }
}

#endif