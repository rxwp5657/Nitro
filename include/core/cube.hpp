#ifndef CUBE_H
#define CUBE_H

#include <math.h>
#include <vector>
#include "../graphics/model.hpp"
#include "../graphics/mesh.hpp"
#include "../graphics/texture.hpp"
#include "./actor.hpp"

namespace nitro
{
    namespace core
    {
        class Cube : public Actor
        {
        public:
            Cube(float side);
            Cube();
        private:
            float side_;

            nitro::graphics::Model GenerateModel(float side) const;
            std::vector<nitro::graphics::Vertex> Vertices(float side) const;
            std::vector<unsigned int> Indices() const;
        };
    }
}

#endif