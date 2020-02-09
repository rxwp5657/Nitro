#ifndef SCENE_H
#define SCENE_H

#include <vector>
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
            Scene(const std::vector<Actor>& actors,
                  const std::vector<PointLight>& lights,
                  const Camera& camera);
            
            Scene();

            std::vector<Actor> Actors() const;
            std::vector<PointLight> Lights() const;
            
            void AddActor(const Actor& actor);
            void AddLight(const PointLight& light);
            
            Camera  SceneCamera() const;
            Camera* CameraPtr();

        private:
            std::vector<Actor> actors_;
            std::vector<PointLight> lights_;
            Camera camera_;
        };
    }
}

#endif