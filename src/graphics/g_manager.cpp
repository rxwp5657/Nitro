#include <g_manager.hpp>

namespace nitro
{
    namespace graphics
    {
        Manager::Manager(const Window& window, const Shader& shader)
        : context_{Context::get_instance()},
          window_{window},
          active_program_{shader},
          shaders_{std::vector<Shader>{shader}}
        {
            
        }

        GLFWwindow* Manager::get_window() const
        {
            return window_.get_window_ptr();
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