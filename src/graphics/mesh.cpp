#include <mesh.hpp>

namespace nitro
{
    namespace graphics
    {
        Mesh::Mesh(const std::vector<Vertex>& vertices, 
                   const std::vector<unsigned int>& indices,
                   const std::vector<Texture>&  textures,
                   const Material& material)
        : vertices_{vertices},
          indices_{indices},
          textures_{textures},
          material_{material},
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

            shader.PosAttrib("aPosition", 3, GL_FLOAT, sizeof(Vertex),  0);
            shader.PosAttrib("aNormal",   3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, normal));
            shader.PosAttrib("aTexCoord", 2, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, tex_coord));
            shader.PosAttrib("aTangent",  3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, tangent));
            shader.PosAttrib("aBitangent",3, GL_FLOAT, sizeof(Vertex),  offsetof(Vertex, bitangent));

            // Interger control value for rendering with or without textures
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            loaded_ = true;
        }

        void Mesh::FlipUV()
        {
            for(int i = 0; i < vertices_.size(); i++)
                vertices_[i].tex_coord.y = 1.0f - vertices_[i].tex_coord.y;
        }

        void Mesh::LoadTextures(const Shader& shader)
        {
            int diffuse = 1, specular = 1, normal = 1, height = 1, skybox = 1;

            for(int i = 0; i < textures_.size(); i++)
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
                else if (textures_[i].Name() == "skybox")
                    num = skybox++;

                textures_[i].TextureUnit(GL_TEXTURE0 + i, i, num);
                textures_[i].Draw(shader);
            }

            shader.SetUniformInt("has_textures", textures_.size() != 0 ? 1 : 0);
        }
        
        void Mesh::LoadMaterials(const Shader& shader)
        {
            shader.SetUniform3f("mat_diffuse",      material_.diffuse);
            shader.SetUniform3f("mat_specular",     material_.specular);
            shader.SetUniform3f("mat_ambient",      material_.ambient);
            shader.SetUniformFloat("mat_shininess", material_.shininess);
            shader.SetUniformFloat("mat_reflectiveness", material_.reflectivity);
            shader.SetUniformFloat("mat_refractiveness", material_.refractivity);
        }

        void Mesh::Draw(const Shader& shader, bool default_framebuffer)
        {
            if(!loaded_ || default_framebuffer)
                Setup(shader);

            
            LoadTextures(shader);
            LoadMaterials(shader);

            glBindVertexArray(vao_);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer_);

            glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, (void*)0);

            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
}