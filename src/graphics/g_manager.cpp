#include <g_manager.hpp>

namespace nitro
{
    namespace graphics
    {
        Manager::Manager(const Context* context,
                         const Window& window)
        : context_{context},
          window_{window},
          shaders_{}
        {
            shaders_["point_lighting"] = Shader{"point_light.vert","point_light.frag"};
            shaders_["spot_lighting"]  = Shader{"spot_light.vert","spot_light.frag"};
            shaders_["directional_lighting"] = Shader{"directional_light.vert","directional_light.frag"};

            shaders_["point_shadows"] = Shader{"point_shadows.vert","point_shadows.frag", "point_shadows.glsl"};
            //shaders_["spot_shadows"]  = Shader{"spot_shadow.vert","spot_shadow.frag"};
            //shaders_["directional_shadows"] = Shader{"directional_shadow.vert","directional_shadow.frag"};

            for(const auto& shader: shaders_)
            {   
                if(shader.second.Status().status_code != nitro::utils::StatusCode::OK)
                {
                    throw std::runtime_error(shader.second.Status().message);
                }
            }
        }

        Manager::~Manager()
        {
            window_.Destroy();
            for(auto& shader : shaders_)
                shader.second.Delete();
        }

        const Window* Manager::get_window() const
        {
            return &window_;
        }

        void Manager::UpdateScene(core::Scene scene)
        {  
            scene.Draw(shaders_, window_.Width(), window_.Height()); 
            glfwSwapBuffers(window_.get_window_ptr());
        }

        void Manager::AddShader(const std::string& name, const Shader& shader)
        {
            if(shader.Status().status_code != nitro::utils::StatusCode::OK)
                throw std::runtime_error(shader.Status().message);
            
            shaders_[name] = shader;
        }
    }
}