#ifndef GMANAGER_H
#define GMANAGER_H

#include <memory>
#include <vector>
#include <context.hpp>
#include <window.hpp>
#include <shader.hpp>
#include <drawable.hpp>

namespace nitro
{
    namespace graphics
    {
        class Manager
        {
        public:
            Manager(const Window& window, const Shader& shader);
            GLFWwindow* get_window() const;
            
            template <typename Actor>
            void UpdateScene(std::vector<Actor>& actors)
            {
                assert((std::is_base_of<Drawable,Actor>::value));

                active_program_.Use();

                glEnable(GL_DEPTH_TEST);  

                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                for(auto& actor : actors)
                {
                    //active_program_.SetUniformMat4("model",actor.Transform());
                    actor.Draw(active_program_);
                }

                glfwSwapBuffers(window_.get_window_ptr());
            }
            
        private:
            std::unique_ptr<Context> context_;
            Window window_;
            Shader active_program_;
            std::vector<Shader> shaders_;
        };
    }
}

#endif
