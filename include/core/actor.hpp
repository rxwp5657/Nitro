//
//  actor.hpp
//  Nitro
//
//  Created by Juan Carlos Sanchez Ruiz de Chavez on 01/06/20.
//  Copyright © 2020 Juan Carlos Sanchez Ruiz. All rights reserved.
// 

#ifndef ACTOR_H
#define ACTOR_H

#include <string>
#include <vector>
#include <initializer_list>
#include <mat4.hpp>
#include <transforms.hpp>
#include "../graphics/model.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/drawable.hpp"

/*
    Basically, an actor is every entity that participates on a scene like a
    sphere, trees, soldiers, etc. 

    Currently, an actor should be capable of appearing on a scene. Thus,
    requiring a model that contains all vertex data (vertices, normals,
    tangents, bitangents, etc.). However, the actor remains having a "reference" 
    to a shader as the first attempt of forward rendering was based on the idea 
    of effects being implemented on self-contained shaders. For example, if an 
    actor needed to implement parallax occlusion it would need to add the 
    "parallax-occlusion" reference to the list of shaders so on the rendering pass
    the effect would only be applied to those objects that have the "reference" to 
    the shader.

    However, after changing the render path to forward rendering per light the 
    list of references is not used anymore. On the other hand, until deferred
    rendering is implemented or the structure of materials is not revised 
    (adding control uniforms of wheter or not a special effect should be applied) 
    the class will keep the list of special efects to be applied.
*/

namespace nitro
{
    namespace core
    {
        class Actor : public graphics::Drawable
        {
        public:
            Actor(const std::string& model,
                  std::initializer_list<std::string> shaders = {"lighting"});
            
            Actor(const graphics::Model& model,
                  std::initializer_list<std::string> shaders = {"lighting"});

            /*
                "Interface" for applying common transforms to the actor's model, this set
                of functions will be used when using the GUI for changing the position (Translation),
                orientation (Rotation), size (Scaling) and Color of the actor's model.

                However, this set of function is still not complete as the following 
                set of attributes need to be accessible:

                + Material Properties:
                    - Specular.
                    - Ambient.
                    - Shininess.
                    - Reflectivity.
                    - Refractivity.
                
                + Texture Properties:
                    - Enabling texture.
                    - Disabling a texture.
                    - Adding a texure.
                    - Removing a texure
            */

            void Rotate(float x, float y, float z);
            void Scale (float x, float y, float z);
            void Translate(float x, float y, float z);
            void Color(float r, float g, float b); // changes diffuse term value
            
            /*
                Some models have their texture coordinate systems with the origin 
                on the upper left corner of the texture image. However, OpenGL
                expects the origin to be on the upper left corner, so if an artifact
                related to texture mapping appears, you can use this function to toggle
                the coordinate system.

                (if the origin is on the upper left corer) FlipUV() -> (origin on the lower left corner)
                (if the origin is on the lower left corer) FlipUV() -> (origin on the upper left corner)
            */

            void FlipUV();

            void Erase() override;
            void Setup(const graphics::Shader& shader) override;
            void Draw(const graphics::Shader& shader,bool default_framebuffer = true)  override;

            void AddShader(const std::string& shader_name);
            std::vector<std::string> Shaders() const;

        protected:
            graphics::Model          model_;
            clutch::Mat4<float>      translation_;
            clutch::Mat4<float>      rotation_;
            clutch::Mat4<float>      scaling_;
            std::vector<std::string> shaders_;

            clutch::Mat4<float> Model()     const;
            clutch::Mat4<float> NormalMat() const;
        };
    }
}


#endif