#include <g_manager.hpp>

namespace nitro
{
    namespace graphics
    {
        Manager::Manager(const Context* context,
                         const Window& window, 
                         const Shader& shader)
        : context_{context},
          window_{window},
          shaders_{{"lighting", shader}, {"light", Shader{"light.vert","light.frag"}}}
        {
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
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
            
            for(auto& actor : scene.Actors())
            {
                for(const auto& shader_name : actor->Shaders())
                {
                    if(shaders_.find(shader_name) != shaders_.end())
                    {
                        auto shader = shaders_.at(shader_name);

                        shader.Use();
                        scene.LoadLights();
                        scene.SceneCamera().Draw(shader);
                        actor->Draw(shader);
                    }
                }
            }
            glfwSwapBuffers(window_.get_window_ptr());
        }
    }
    
}