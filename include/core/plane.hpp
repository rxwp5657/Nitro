#include <math.h>
#include <iostream>
#include <vector>
#include <model.hpp>
#include <mesh.hpp>
#include <texture.hpp>
#include <actor.hpp>

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