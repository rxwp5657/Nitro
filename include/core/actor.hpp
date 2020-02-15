#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <drawable.hpp>
#include <model.hpp>
#include <mat4.hpp>
#include <shader.hpp>
#include <transforms.hpp>

namespace nitro
{
    namespace core
    {
        class Actor : public graphics::Drawable
        {
        public:
            Actor(const std::string& model);

            void Rotate(const clutch::Mat4<float> r);
            void Scale (const clutch::Mat4<float> s);
            void Translate(const clutch::Mat4<float> t);
            
            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader)  override;

        private:
            graphics::Model model_;
            clutch::Mat4<float> translation_;
            clutch::Mat4<float> rotation_;
            clutch::Mat4<float> scaling_;

            clutch::Mat4<float> Model()     const;
            clutch::Mat4<float> NormalMat() const;
        };
    }
}


#endif