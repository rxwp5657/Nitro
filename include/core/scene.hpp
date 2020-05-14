#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <map>
#include <actor.hpp>
#include <camera.hpp>
#include <skybox.hpp>
#include <shader.hpp>
#include <framebuffer.hpp>
#include <spot_light.hpp>
#include <point_light.hpp>
#include <directional_light.hpp>
#include <debugger.hpp>

namespace nitro
{
    namespace core
    {
        class Scene
        {
        public:
            Scene();

            Scene(const std::vector<std::shared_ptr<Actor>>      actors,
                  const std::vector<std::shared_ptr<PointLight>> point_lights,
                  const std::vector<std::shared_ptr<SpotLight>>  spot_lights,
                  const std::vector<std::shared_ptr<DirectionalLight>> dir_lights,
                  const Camera& camera,
                  const Skybox& skybox);

            std::vector<std::shared_ptr<Actor>>      Actors()      const;
            std::vector<std::shared_ptr<PointLight>> PointLights() const;
            Camera  SceneCamera() const;
            Camera* CameraPtr();
                      
            void AddSkyBox(const Skybox& skybox);
            void AddActor(const std::shared_ptr<Actor> actor);
            void AddPointLight(const std::shared_ptr<PointLight> light);
            void AddSpotLight(const std::shared_ptr<SpotLight>   light);
            void AddDirectionalLight(const std::shared_ptr<DirectionalLight> light);
            
            void Draw(const std::map<std::string, graphics::Shader>& shaders, int screen_width, int screen_height);

        private:
            std::vector<std::shared_ptr<Actor>>      actors_;
            std::vector<std::shared_ptr<PointLight>> point_lights_;
            std::vector<std::shared_ptr<SpotLight>>  spot_lights_;
            std::vector<std::shared_ptr<DirectionalLight>> dir_lights_;
            std::vector<graphics::Texture> gbuffer_textures_;
            Camera camera_;
            Skybox skybox_;
            graphics::Framebuffer gbuffer_; 
            graphics::Framebuffer shadow_buffer_;

            void DrawActors(const graphics::Shader& shader);
            void DrawSkyBox(const graphics::Shader& shader);
            
            void Setup() const;

            void ForwardRender(const  std::map<std::string, graphics::Shader>& shaders);
            void DeferredRender(const std::map<std::string, graphics::Shader>& shaders);
            void RenderShadows(const  std::map<std::string, graphics::Shader>& shaders);

            void EnableMultipass();
            void DisableMultipass();

            void SetDefaultViewPort(int width, int height);
            void ClearBuffers();

            template <typename T>
            void ForwardRenderLights(const graphics::Shader& light_shader, const std::vector<T>& lights)
            {
                light_shader.Use();
                
                for(const auto& light : lights)
                {
                    camera_.Draw(light_shader);
                    light->Draw(light_shader);
                    DrawActors(light_shader);
                }
            }

            template <typename T>
            void ForwardRenderShadows(const graphics::Shader& shadow_shader, const std::vector<T>& lights)
            {                
                for(const auto& light : lights)
                {
                    if(light->Shadow())
                    {
                        shadow_shader.Use();
                        light->DrawShadows(shadow_shader); 
                        DrawActors(shadow_shader);
                        glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    }
                } 
            }
        };
    }
}

#endif