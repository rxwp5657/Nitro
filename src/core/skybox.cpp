#include <skybox.hpp>

namespace nitro
{
    namespace core
    {
        Skybox::Skybox()
        : Actor{GenerateModel("", {})}
        {
        }

        Skybox::Skybox(const std::string& folder, const std::vector<std::string>& texture_names)
        : Actor{GenerateModel(folder,texture_names)}
        {
        }

        void Skybox::Draw(const  graphics::Shader& shader, bool default_framebuffer)
        {
            glDepthFunc(GL_LEQUAL);
            Setup(shader);
            model_.Draw(shader);
            glDepthFunc(GL_LESS);
        }

        void Skybox::Bind(graphics::Shader& shader) const
        {
            glActiveTexture(GL_TEXTURE10);
            shader.SetUniformInt("skybox1", 10);
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
        }
        
        void Skybox::Unbind() const
        {
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }

        std::vector<nitro::graphics::Vertex> Skybox::Vertices() const
        {
            std::vector<nitro::graphics::Vertex> result{};

            clutch::Vec3<float> p1{1.0f, 1.0f, 1.0f};
            clutch::Vec3<float> p2{-1.0f, 1.0f, 1.0f};
            clutch::Vec3<float> p3{-1.0f, -1.0f, 1.0f};
            clutch::Vec3<float> p4{1.0f, -1.0f, 1.0f};
            clutch::Vec3<float> p5{1.0f, 1.0f, -1.0f};
            clutch::Vec3<float> p6{-1.0f, 1.0f, -1.0f};
            clutch::Vec3<float> p7{-1.0f, -1.0f, -1.0f};
            clutch::Vec3<float> p8{1.0f, -1.0f, -1.0f};

            //Front face
            result.push_back({p1,{},{},{},{}});
            result.push_back({p2,{},{},{},{}});
            result.push_back({p3,{},{},{},{}});
            result.push_back({p4,{},{},{},{}});

            // Top face
            result.push_back({p1,{},{},{},{}});
            result.push_back({p5,{},{},{},{}});
            result.push_back({p6,{},{},{},{}});
            result.push_back({p2,{},{},{},{}});
            
            // Left face
            result.push_back({p2,{},{},{},{}});
            result.push_back({p6,{},{},{},{}});
            result.push_back({p7,{},{},{},{}});
            result.push_back({p3,{},{},{},{}});
            
            // Right face
            result.push_back({p5,{},{},{},{}});
            result.push_back({p1,{},{},{},{}});
            result.push_back({p4,{},{},{},{}});
            result.push_back({p8,{},{},{},{}});
            
            // Back face
            result.push_back({p5,{},{},{},{}});
            result.push_back({p6,{},{},{},{}});
            result.push_back({p7,{},{},{},{}});
            result.push_back({p8,{},{},{},{}});

            // Bottom face
            result.push_back({p4,{},{},{},{}});
            result.push_back({p8,{},{},{},{}});
            result.push_back({p7,{},{},{},{}});
            result.push_back({p3,{},{},{},{}});
            
            return result;
        }

        std::vector<unsigned int> Skybox::Indices() const 
        {
            std::vector<unsigned int> result{};

            for(int i = 0; i < 24; i += 4)
            {
                // First triangle
                result.push_back(i);
                result.push_back(i+1);
                result.push_back(i+2);

                // Second triangle
                result.push_back(i);
                result.push_back(i+2);
                result.push_back(i+3);
            }

            return result;
        }

        nitro::graphics::Model Skybox::GenerateModel(const std::string& folder,const std::vector<std::string>& texture_names)
        {
            std::vector<nitro::graphics::Vertex> vertices{Vertices()};
            std::vector<unsigned int> indices{Indices()};
            std::vector<nitro::graphics::Texture> textures{};

            if(texture_names.size() != 0)
            {
                nitro::graphics::Texture texture{folder,texture_names,"skybox"}; 
                texture_ = texture.TextureReference();
                textures.push_back(texture);
                
            }
            
            nitro::graphics::Material material{{1.0f, 0.2f, 1.0f},
                                               {0.9f, 0.9f, 0.9f}, 
                                               {0.1f, 0.1f, 0.1f},
                                               200.0f, 0.0f, 0.0f};

            nitro::graphics::Mesh mesh{vertices, indices, textures, material};

            return nitro::graphics::Model{{mesh}};
        }
    }
}