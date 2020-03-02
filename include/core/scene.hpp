#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <actor.hpp>
#include <camera.hpp>
#include <light.hpp>

namespace nitro
{
    namespace core
    {
        class Scene
        {
        public:
            Scene(const std::vector<std::shared_ptr<Actor>>      actors,
                  const std::vector<std::shared_ptr<PointLight>> point_lights,
                  const Camera& camera);
            
            Scene();

            std::vector<std::shared_ptr<Actor>>      Actors() const;
            std::vector<std::shared_ptr<PointLight>> PointLights() const;
                        
            void AddActor(const std::shared_ptr<Actor>      actor);
            void AddLight(const std::shared_ptr<PointLight> light);

            void LoadLights() const;
            
            Camera  SceneCamera() const;
            Camera* CameraPtr();

        private:
            std::vector<std::shared_ptr<Actor>>      actors_;
            std::vector<std::shared_ptr<PointLight>> point_lights_;
            Camera camera_;
            GLuint light_block_buffer_;
        };
    }
}

#endif