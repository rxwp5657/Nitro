#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include <vector>
#include <vec3.hpp>
#include <model.hpp>
#include <shader.hpp>
#include <drawable.hpp>
#include <transforms.hpp>

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
                       const float max_distance);
            
            PointLight();

            void Rotate(const clutch::Mat4<float> t);
            void Scale (const clutch::Mat4<float> t);
            void Translate(const clutch::Mat4<float> t);

            void AddShader(const std::string& shader_name);
            std::vector<std::string> Shaders() const;

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader)  override;

        private:
            graphics::Model model_;
            clutch::Vec3<float> position_;
            clutch::Vec3<float> color_;
            float max_distance_;

            clutch::Mat4<float> translation_;
            clutch::Mat4<float> rotation_;
            clutch::Mat4<float> scaling_;

            std::vector<std::string> shaders_;

            clutch::Mat4<float> Model() const;
        };
    }
}

#endif