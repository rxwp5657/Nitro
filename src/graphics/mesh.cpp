#include <mesh.hpp>

namespace nitro
{
    namespace graphics
    {
        Mesh::Mesh(const std::vector<Vertex>& vertices, 
                   const std::vector<unsigned int>& indices,
                   const std::vector<Texture>&  textures)
        : vertices_{vertices},
          indices_{indices},
          textures_{textures},
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
            shader.PosAttrib("aPosition", 3, GL_FLOAT, sizeof(Vertex),  0);
            // vertex normals
            shader.PosAttrib("aNormal",   3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, normal));
            // vertex texture coords
            shader.PosAttrib("aTexCoord", 2, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, tex_coord));
            // vertex Tangent from tangent space
            shader.PosAttrib("aTangent",  3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, tangent));
            // vertex Bitangent from tangent space
            shader.PosAttrib("aBitangent",3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, bitangent));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            loaded_ = true;
        }

        void Mesh::FlipUV()
        {
            for(int i = 0; i < vertices_.size(); i++)
                vertices_[i].tex_coord.y = 1.0f - vertices_[i].tex_coord.y;
        }

        void Mesh::Draw(const Shader& shader)
        {
            if(!loaded_)
                Setup(shader);

            int diffuse = 1, specular = 1, normal = 1, height = 1;

            for(int i = 0; i <textures_.size(); i++)
            {
                int num = 0;
                if(textures_[i].Name() == "texture_diffuse")
                    num = diffuse++;
                else if (textures_[i].Name() == "texture_specular")
                    num = specular++;
                else if (textures_[i].Name() == "texture_normal")
                    num = normal++;
                else if (textures_[i].Name() == "texture_height")
                    num = height++;

                textures_[i].TextureUnit(GL_TEXTURE0 + i, i, num);
                textures_[i].Draw(shader);
            }

            glBindVertexArray(vao_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_);

            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);

            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
            glActiveTexture(GL_TEXTURE0);
        }
    }
}