#include "../../include/core/point_light.hpp"

namespace nitro
{
    namespace core
    {
        PointLight::PointLight(const clutch::Vec4<float>& position,
                               const clutch::Vec4<float>& color,
                               const float max_distance)
        : Shadow{4, 5},
          position_{position},
          color_{color},
          transform_{},
          max_distance_{max_distance}
        {
        
        }

        PointLight::PointLight()
        : Shadow{4, 5},
          position_{0.0f, 0.0f, 0.0f, 1.0f},
          color_{1.0f, 1.0f, 1.0f, 1.0f},
          transform_{},
          max_distance_{45.0f}
        {

        }

        void PointLight::Range(float range)
        {
            max_distance_ = range;
        }

        void PointLight::Move(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        clutch::Mat4<float> PointLight::FaceTransform(const clutch::Vec4<float>& direction, const clutch::Vec4<float>& up)
        {
            return clutch::LookAt(position_, position_ + direction, up);
        }

        void PointLight::SetupShadows()
        {   
            shadow_maps_[0] = graphics::Texture{"shadow_map_front", constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR};
            shadow_maps_[1] = graphics::Texture{"shadow_map_rear",  constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR};
            shadow_maps_[2] = graphics::Texture{"image",            constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR};
            shadow_maps_[3] = graphics::Texture{"depth",            constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_NEAREST};

            framebuffers_[0].Size(constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);
            framebuffers_[0].AttachTexture(shadow_maps_[3], GL_DEPTH_STENCIL_ATTACHMENT);

            if(!framebuffers_[0].Complete())
                throw std::runtime_error("Point light framebuffer is not complete \n");

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, 0);
            shadow_maps_[1].TextureUnit(GL_TEXTURE13, 13, 0);

            set_up_= true;
        }

        void PointLight::DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                     const std::vector<std::shared_ptr<Actor>>& actors)
        {    
            auto shader = shaders.at("point_shadows");
        
            if(!set_up_) SetupShadows();

            transform_ = clutch::LookAt(position_, position_ + clutch::Vec4<float>{0.0f, 0.0f, 1.0, 0.0f}, clutch::Vec4<float>{0.0f, 1.0f, 0.0f, 0.0f});

            ShadowPass(shader, framebuffers_[0], shadow_maps_[0], transform_,  1.0f, actors);
            ShadowPass(shader, framebuffers_[0], shadow_maps_[1], transform_, -1.0f, actors);

            PostProcess(shaders);
        }

        void PointLight::ShadowPass(const graphics::Shader&      shader, 
                                    const graphics::Framebuffer& framebuffer, 
                                    const graphics::Texture&     shadow_map, 
                                    const clutch::Mat4<float>    view,
                                    const float                  dir,
                                    const std::vector<std::shared_ptr<Actor>>& actors)
        {
            shader.Use();

            framebuffer.AttachTexture(shadow_map, GL_COLOR_ATTACHMENT0);
            framebuffer.Bind();

            shader.SetUniformMat4("uView",  view);
            shader.SetUniformFloat("uFar",  max_distance_);
            shader.SetUniformFloat("uDir",  dir);
            shader.SetUniformFloat("uNear", 0.5f);

            for(const auto& actor : actors)
                actor->Draw(shader, false);
        }

        void PointLight::PostProcess(const std::map<std::string, graphics::Shader>& shaders)
        {
            auto shader = shaders.at("gaussian");
            
            GaussianBlur blur{constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT};
            
            blur.Process(shader, shadow_maps_[0], shadow_maps_[2]);
            blur.Process(shader, shadow_maps_[1], shadow_maps_[2]);

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, "shadow_map_front");
            shadow_maps_[1].TextureUnit(GL_TEXTURE13, 13, "shadow_map_rear");
        }

        void PointLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("uLightPos",       position_);
            shader.SetUniform4f("uLightColor",     color_);
            shader.SetUniformFloat("uMaxDistance", max_distance_);
            shader.SetUniformInt("uCastsShadow",   cast_shadows_);
            shader.SetUniformInt("uPCF",           pcf_);
            shader.SetUniformMat4("uDPView",       transform_);
            shader.SetUniformFloat("uNearPlane",   0.5f);

            shadow_maps_[0].Draw(shader);
            shadow_maps_[1].Draw(shader);

            /*
            if(cast_shadows_)
                shadow_maps_[0].Draw(shader);
            else
            {
                glActiveTexture(GL_TEXTURE12);
                shader.SetUniformInt("shadow_map", 12);
                glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_maps_[0].TextureReference());
            }
            */
        }

        void PointLight::Erase()
        {

        }

        void PointLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
