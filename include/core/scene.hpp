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

            std::vector<Actor> Actors() const;
            std::vector<PointLight> Lights() const;
            Camera SceneCamera() const;

        private:
            std::vector<Actor> actors_;
            std::vector<PointLight> lights_;
            Camera camera_;
        };
    }
}

#endif