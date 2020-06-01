#ifndef SPHERE_H
#define SPHERE_H

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
        class Sphere : public Actor
        {
        public: 
            Sphere(float radius, int stacks = 40, int sectors = 80);
            Sphere();

            /*
            float Radius();
            void  Radius(float radius);

            int  Stacks();
            void Stacks(int stacks);

            int  Sectors();
            void Sectors(int sectors);
            */
        private:
            float radius_;
            int   stacks_;
            int   sectors_;

            std::vector<nitro::graphics::Vertex> Vertices(float radius, int stacks, int sectors) const;
            std::vector<unsigned int> Indices(int stacks, int sectors) const;
            nitro::graphics::Model GenerateModel(float radius, int stacks, int sectors) const;
        };
    }
}

#endif