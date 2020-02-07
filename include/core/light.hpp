#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vec3.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <drawable.hpp>

namespace nitro
{
    namespace core
    {
        class PointLight : public graphics::Drawable
        {
        public:
            PointLight(const std::string& model,
                       const clutch::Vec3<float>& position, 
                       const clutch::Vec3<float>& color,
                       const float max_distance,
                       clutch::Mat4<float> transform = clutch::Mat4<float>{});
            
            clutch::Mat4<float> Transform() const;
            void Transform(const clutch::Mat4<float> t);

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader)  override;

        private:
            graphics::Model model_;
            clutch::Vec3<float> position_;
            clutch::Vec3<float> color_;
            float max_distance_;
            clutch::Mat4<float> transform_;
        };
    }
}

#endif