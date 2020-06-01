#ifndef PLANE_H
#define PLANE_H

#include <math.h>
#include <iostream>
#include <vector>

#include "../graphics/model.hpp"
#include "../graphics/mesh.hpp"
#include "../graphics/texture.hpp"
#include "./actor.hpp"

namespace nitro
{
    namespace core
    {
        enum class PlaneType
        {
            XY,
            XZ,
            YZ
        };

        class Plane : public Actor
        {
        public:
            Plane(PlaneType plane_type);
            Plane();
        private:
            PlaneType type_;

            nitro::graphics::Model GenerateModel(PlaneType plane_type) const;
            std::vector<nitro::graphics::Vertex> Vertices(PlaneType plane_type,const clutch::Vec3<float> normal) const;
            std::vector<unsigned int> Indices() const;
        };
    }
}

#endif