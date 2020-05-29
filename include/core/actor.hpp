#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <initializer_list>
#include <model.hpp>
#include <mat4.hpp>
#include <shader.hpp>
#include <drawable.hpp>
#include <transforms.hpp>

namespace nitro
{
    namespace core
    {
        class Actor : public graphics::Drawable
        {
        public:
            Actor(const std::string& model,
                  std::initializer_list<std::string> shaders = {"lighting"});
            
            Actor(const graphics::Model& model,
                  std::initializer_list<std::string> shaders = {"lighting"});

            void Rotate(float x, float y, float z);
            void Scale (float x, float y, float z);
            void Translate(float x, float y, float z);
            void Color(float r, float g, float b);
            
            void FlipUV();

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader,bool default_framebuffer = true)  override;

            void AddShader(const std::string& shader_name);
            std::vector<std::string> Shaders() const;

        protected:
            graphics::Model          model_;
            clutch::Mat4<float>      translation_;
            clutch::Mat4<float>      rotation_;
            clutch::Mat4<float>      scaling_;
            std::vector<std::string> shaders_;

            clutch::Mat4<float> Model()     const;
            clutch::Mat4<float> NormalMat() const;
        };
    }
}


#endif