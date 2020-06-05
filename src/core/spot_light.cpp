#include "../../include/core/spot_light.hpp"

namespace nitro
{
    namespace core
    {
        SpotLight::SpotLight()
        : Shadow{3,5},
          position_{0.0f, 0.0f, 0.0f, 1.0f},
          direction_{0.0f,0.0f,-1.0f, 1.0f},
          color_{1.0f,1.0f,1.0f,1.0f},
          transform_{},
          projection_{},
          cutoff_{cos((clutch::PI   * 65.0f) / 180.0f)},
          max_distance_{20.0f},
          umbra_{cos((clutch::PI    * 40.5f) / 180.0f)},
          penumbra_{cos((clutch::PI * 20.5f) / 180.0f)}
        { 

        }

        SpotLight::SpotLight(clutch::Vec4<float> position,
                             clutch::Vec4<float> direction,
                             clutch::Vec4<float> color,
                             float cutoff,
                             float umbra,
                             float penumbra,
                             float max_distance)
        : Shadow{2,5}, 
          position_{position},
          direction_{direction},
          color_{color},
          transform_{},
          projection_{},
          cutoff_{cutoff},
          max_distance_{max_distance},
          umbra_{umbra},
          penumbra_{penumbra}
        {

        }

        void SpotLight::Umbra(float umbra)
        {
          umbra_ = umbra;
        }

        void SpotLight::Penumbra(float penumbra)
        {
          penumbra_ = penumbra;
        }

        void SpotLight::Distance(float distance)
        {
            max_distance_ = distance;
        }

        void SpotLight::Cutoff(float cutoff)
        {
            cutoff_ = cutoff;
        }

        void SpotLight::MovePos(float x, float y, float z)
        {
            position_ = clutch::Translation(x,y,z) * position_;
        }

        void SpotLight::MoveDir(float x, float y, float z)
        {
            direction_ = clutch::Translation(x,y,z) * direction_;
        }

        void SpotLight::SetupShadows()
        {
            shadow_maps_[0]  = graphics::Texture{"shadow_map", constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR};
            shadow_maps_[1]  = graphics::Texture{"image",      constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_RG32F, GL_RGBA, GL_FLOAT, GL_LINEAR};
            shadow_maps_[2]  = graphics::Texture{"depth",      constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT, GL_TEXTURE_2D, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, GL_NEAREST};

            framebuffers_[0].Size(constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT);

            framebuffers_[0].AttachTexture(shadow_maps_[0], GL_COLOR_ATTACHMENT0);
            framebuffers_[0].AttachTexture(shadow_maps_[2], GL_DEPTH_STENCIL_ATTACHMENT);

            if(!framebuffers_[0].Complete())
                throw std::runtime_error("Spot light framebuffer is not complete \n");

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, 0);
            set_up_ = true;
        }

        void SpotLight::DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                                    const std::vector<std::shared_ptr<Actor>>& actors)
        {
            auto shader = shaders.at("spot_shadows");
            shader.Use();

            if(!set_up_) SetupShadows();

            framebuffers_[0].Bind();
            //glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            
            projection_ = clutch::Perspective((90.0f * clutch::PI) / 180.0f, 1.5f, 1.0f, max_distance_);
            transform_  = clutch::LookAt(position_, position_ + direction_, clutch::Vec4<float>{0.0f, 1.0f, 0.0f, 0.0f}); 

            shader.SetUniformMat4("uLightTransform", projection_ * transform_);

            for(const auto& actor : actors)
                actor->Draw(shader, false);

            PostProcess(shaders);
        }

        void SpotLight::PostProcess(const std::map<std::string, graphics::Shader>& shaders)
        {
            auto shader = shaders.at("gaussian");
            
            GaussianBlur blur{constants::SHADOW_WIDTH, constants::SHADOW_HEIGHT};
            blur.Process(shader, shadow_maps_[0], shadow_maps_[1]);

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, "shadow_map");

            framebuffers_[0].AttachTexture(shadow_maps_[0], GL_COLOR_ATTACHMENT0);
            
            /*
            framebuffers_[1].AttachTexture(shadow_maps_[1], GL_COLOR_ATTACHMENT0);
            
            framebuffers_[1].Bind();

            shader.Use();
            shader.SetUniformInt("vertical", false);
            shadow_maps_[0].TextureUnit(GL_TEXTURE0, 0, "image");
            shadow_maps_[0].Draw(shader);
            shadow_maps_[0].DrawQuad();

            shadow_maps_[0].TextureUnit(GL_TEXTURE12, 12, "shadow_map");
            
            shader.Disable();
            framebuffers_[1].Unbind();

            framebuffers_[1].AttachTexture(shadow_maps_[0], GL_COLOR_ATTACHMENT0);

            framebuffers_[1].Bind();

            shader.Use();
            shader.SetUniformInt("vertical", true);
            shadow_maps_[1].TextureUnit(GL_TEXTURE0, 0, "image");
            shadow_maps_[1].Draw(shader);
            shadow_maps_[1].DrawQuad();

            shader.Disable();
            framebuffers_[1].Unbind();
            */
        }

        void SpotLight::Draw(const graphics::Shader& shader, bool default_framebuffer)
        {
            shader.SetUniform4f("uLightPos",          position_);
            shader.SetUniform4f("uLightDir",          direction_);
            shader.SetUniform4f("uLightColor",        color_);
            shader.SetUniformMat4("uLightTransform",  projection_ * transform_);
            shader.SetUniformFloat("uCutoff",         cutoff_);
            shader.SetUniformFloat("uMaxDistance",    max_distance_);
            shader.SetUniformFloat("uUmbra",          umbra_);
            shader.SetUniformFloat("uPenumbra",       penumbra_);
            shader.SetUniformInt("uCastsShadow",      cast_shadows_);
            shader.SetUniformInt("uPCF",              pcf_);

            shadow_maps_[0].Draw(shader);
        }

        void SpotLight::Erase()
        {
        
        }

        void SpotLight::Setup(const graphics::Shader& shader)
        {

        }
    }
}
