#ifndef SPOT_LIGHT
#define SPOT_LIGHT

#include <memory>
#include <vec4.hpp>
#include <mat4.hpp>
#include <transforms.hpp>
#include <texture.hpp>
#include <shader.hpp>
#include <lookat.hpp>
#include <drawable.hpp>
#include <constants.hpp>
#include <projections.hpp>
#include <shadows.hpp>
#include <actor.hpp>

namespace nitro 
{
    namespace core
    {
        class SpotLight : public graphics::Drawable, public Shadow
        {
        public:

            SpotLight();

            SpotLight(clutch::Vec4<float> position,
                      clutch::Vec4<float> direction,
                      clutch::Vec4<float> color = {1.0f,1.0f,1.0f,1.0f},
                      float cutoff   = cos((clutch::PI * 65.0f) / 180.0f),
                      float umbra    = cos((clutch::PI * 40.5f) / 180.0f),
                      float penumbra = cos((clutch::PI * 20.5f) / 180.0f),
                      float max_distance = 5.0f);

            void Umbra(float umbra);
            void Penumbra(float penumbra);
            void Distance(float distance);
            void Cutoff(float cutoff);

            void MovePos(float x, float y, float z);
            void MoveDir(float x, float y, float z);

            void Draw(const graphics::Shader& shader, bool default_framebuffer = true) override;
            void Erase() override;

            void DrawShadows(const std::map<std::string, graphics::Shader>& shaders,
                             const std::vector<std::shared_ptr<Actor>>& actors) override; 
            void SetupShadows() override;

        private:
            clutch::Vec4<float> position_;
            clutch::Vec4<float> direction_;
            clutch::Vec4<float> color_;
            clutch::Mat4<float> transform_;
            clutch::Mat4<float> projection_;
            float cutoff_;
            float max_distance_;
            float umbra_;
            float penumbra_;

            void Setup(const graphics::Shader& shader) override;
        };
    }
}


#endif