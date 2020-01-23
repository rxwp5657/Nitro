#include <mesh.hpp>

namespace nitro
{
    namespace graphics
    {
        Mesh::Mesh(const std::vector<Vertex>& vertices, 
                       const std::vector<unsigned int>& indices,
                       const std::vector<TextureInfo>&  textures_info)
        : vertices_{vertices},
          indices_{indices},
          textures_info_{textures_info},
          loaded_{false}
        {
        
        }

        void Mesh::Erase() 
        {

        }

        void Mesh::Setup(const Shader& shader)
        {
            glGenVertexArrays(1, &vao_);
            glBindVertexArray(vao_);

            glGenBuffers(1, &elementbuffer_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_);  
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

            glGenBuffers(1, &vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

            //Link Data to Shader Variables

            // vertex positions
            shader.PosAttrib("position", 3,GL_FLOAT, sizeof(Vertex),offsetof(Vertex, position));
            // vertex normals
            shader.PosAttrib("normal", 3,GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, normal));
            // vertex texture coords
            shader.PosAttrib("texture",2,GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, tex_coord));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            loaded_ = true;
        }

        void Mesh::Draw(const Shader& shader)
        {
            if(!loaded_)
                Setup(shader);

            glBindVertexArray(vao_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_);

            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, (void*)0);

            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        }
    }
}