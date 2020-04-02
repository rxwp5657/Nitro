#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <initializer_list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec3.hpp>
#include <actor.hpp>
#include <shader.hpp>
#include <stb_image.h>

namespace nitro
{
    namespace core
    {
        class Skybox : public Actor
        {
        public:

            Skybox(const std::string& folder,const std::vector<std::string>& texture_names);
            Skybox();

            void Bind(graphics::Shader& shader) const;
            void Unbind() const;

            void Draw(const graphics::Shader& shader) override;

        private:
            GLuint texture_;
            std::vector<nitro::graphics::Vertex> Vertices() const;
            std::vector<unsigned int> Indices() const;
            nitro::graphics::Model GenerateModel(const std::string& folder,const std::vector<std::string>& texture_names);
        };
    }
}

#endif