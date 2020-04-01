#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <actor.hpp>
#include <camera.hpp>
#include <light.hpp>
#include <skybox.hpp>

namespace nitro
{
    namespace core
    {
        const int POINT_LIGHTS = 2;
        const int SPOT_LIGHTS  = 2;
        const int DIRECTIONAL_LIGHTS = 2;

        class Scene
        {
        public:
            Scene(const std::vector<std::shared_ptr<Actor>>      actors,
                  const std::vector<std::shared_ptr<PointLight>> point_lights,
                  const std::vector<std::shared_ptr<SpotLight>>  spot_lights,
                  const std::vector<std::shared_ptr<DirectionalLight>> dir_lights,
                  const Camera& camera,
                  const Skybox& skybox);
            
            Scene();

            std::vector<std::shared_ptr<Actor>>      Actors()      const;
            std::vector<std::shared_ptr<PointLight>> PointLights() const;
                        
            void AddActor(const std::shared_ptr<Actor>      actor);
            
            void AddPointLight(const std::shared_ptr<PointLight> light);
            void AddSpotLight(const std::shared_ptr<SpotLight>   light);
            void AddDirectionalLight(const std::shared_ptr<DirectionalLight> light);

            void AddSkyBox(const Skybox& skybox);
            void DrawSkyBox(const graphics::Shader& shader);

            void LoadLights() const;
            
            Camera  SceneCamera() const;
            Camera* CameraPtr();

        private:
            std::vector<std::shared_ptr<Actor>>      actors_;
            std::vector<std::shared_ptr<PointLight>> point_lights_;
            std::vector<std::shared_ptr<SpotLight>>  spot_lights_;
            std::vector<std::shared_ptr<DirectionalLight>> dir_lights_;
            Camera camera_;
            Skybox skybox_;
            GLuint light_buffer_;
            GLuint num_lights_buffer_;

            void LoadNumLights() const;

            int LoadPointLights(int start_offset) const;
            int LoadSpotLights(int start_offset)  const;
            int LoadDirectionalLights(int start_offset) const;
        };
    }
}

#endif