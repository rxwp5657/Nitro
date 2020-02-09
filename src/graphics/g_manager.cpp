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
          active_program_{shader},
          shaders_{std::vector<Shader>{shader}}
        {
            if(active_program_.Status().status_code != nitro::utils::StatusCode::OK)
            {
                throw std::runtime_error(active_program_.Status().message);
            }
        }

        Manager::~Manager()
        {
            window_.Destroy();
            for(auto& shader : shaders_)
                shader.Delete();
        }

        const Window* Manager::get_window() const
        {
            return &window_;
        }

        void Manager::UpdateScene(core::Scene scene)
        {   
            active_program_.Use();
            
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  

            scene.SceneCamera().Draw(active_program_);
            
            for(auto& ligh  : scene.Lights())
            {
                ligh.Draw(active_program_);
            }

            for(auto& actor : scene.Actors())
            {
                actor.Draw(active_program_);
            }

            glfwSwapBuffers(window_.get_window_ptr());
        }
    }
    
}