#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vec3.hpp>
#include <vec2.hpp>
#include <shader.hpp>
#include <drawable.hpp>
#include <texture.hpp>

namespace nitro
{
    namespace graphics
    {
        struct Vertex
        {
            clutch::Vec3<float> position;
            clutch::Vec3<float> normal;
            clutch::Vec2<float> tex_coord;
        };

        class Mesh : public Drawable
        {
        public:
            Mesh(const std::vector<Vertex>& vertices, 
                 const std::vector<unsigned short>& indices,
                 const std::vector<Texture>& textures);

            void Erase() override;
            void Setup(const Shader& shader) override;
            void Draw(const Shader& shader) override;

        private:
            std::vector<Vertex>       vertices_;
            std::vector<unsigned short> indices_;
            std::vector<Texture>  textures_;
            GLuint vbo_;
            GLuint vao_;
            GLuint elementbuffer_;
            bool loaded_;
        };
    }
}

#endif


