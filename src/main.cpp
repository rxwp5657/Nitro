#define  GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mesh.hpp>
#include <shader.hpp>
#include <vector>
#include <string>
#include <vec2.hpp>
#include <vec3.hpp>
#include <utils.hpp>
#include <texture.hpp>
#include <model.hpp>
#include <context.hpp>
#include <window.hpp>
#include <g_manager.hpp>
#include <drawable.hpp>

int main(int argc, char **argv)
{
    auto context = nitro::graphics::Context::get_instance();
    nitro::graphics::Window window{800, 600, "Nitro", nitro::graphics::WindowType::WINDOWED, true};
    nitro::graphics::Shader shader{"nitro.vert", "nitro.frag"};

    if(shader.Status().status_code != nitro::utils::StatusCode::OK)
    {
        std::cout << shader.Status().message << "\n";
    }

    nitro::graphics::Manager manager{window, shader};

    nitro::graphics::Model model{"monkey/monkey.obj"};
    std::vector<nitro::graphics::Model> actors{model};

    while(!glfwWindowShouldClose(manager.get_window()))
    { 
        manager.UpdateScene(actors);

        glfwPollEvents();
        if (glfwGetKey(manager.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(manager.get_window(), GL_TRUE);
    }

    glfwTerminate();
    return 0;
}