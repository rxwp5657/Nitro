#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <drawable.hpp>
#include <model.hpp>
#include <mat4.hpp>
#include <shader.hpp>

namespace nitro
{
    namespace core
    {
        class Actor : public graphics::Drawable
        {
        public:
            Actor(const std::string& model);

            clutch::Mat4<float> Transform() const;
            void Transform(const clutch::Mat4<float> t);
            
            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader)  override;

        private:
            graphics::Model model_;
            clutch::Mat4<float> transform_;
        };
    }
}


#endif