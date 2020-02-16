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
            Scene(const std::vector<std::shared_ptr<Actor>> actors,
                  const Camera& camera);
            
            Scene();

            std::vector<std::shared_ptr<Actor>> Actors() const;            
            void AddActor(const std::shared_ptr<Actor> actor);
            
            Camera  SceneCamera() const;
            Camera* CameraPtr();

        private:
            std::vector<std::shared_ptr<Actor>> actors_;
            Camera camera_;
        };
    }
}

#endif