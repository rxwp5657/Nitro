#define  GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <mesh.hpp>
#include <shader.hpp>
#include <vector>
#include <vec4.hpp>
#include <mat4.hpp>
#include <projections.hpp>
#include <string>
#include <model.hpp>
#include <context.hpp>
#include <window.hpp>
#include <g_manager.hpp>
#include <scene.hpp>

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

    clutch::Vec4<float> posititon{0.0f, 0.0f, 5.0f, 1.0};
    clutch::Vec4<float> target{0.0f, 0.0f, -1.0f, 1.0};
    clutch::Vec4<float> up{0.0f, 1.0f, 0.0f, 0.0};

    clutch::Vec3<float> l_posititon{0.0f,1.5f,0.0};
    clutch::Vec3<float> l_power{3.0f,3.0f,3.0f};

    auto perspective =  clutch::Perspective((45.0f * clutch::PI) / 180, 800.0f / 600.0f, 1.0f, 100.0f);

    nitro::core::PointLight light{"monkey/monkey.obj",l_posititon, l_power,20.0f};
    nitro::core::Actor      model{"monkey/monkey.obj"};
    nitro::core::Camera     camera{posititon, target, up, perspective};

    std::vector<nitro::core::Actor>      actors{model};
    std::vector<nitro::core::PointLight> lights{light};

    nitro::core::Scene scene{actors,lights,camera};

    while(!glfwWindowShouldClose(manager.get_window()))
    { 
        manager.UpdateScene(scene);

        glfwPollEvents();
        if (glfwGetKey(manager.get_window(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(manager.get_window(), GL_TRUE);
    }

    glfwTerminate();
    return 0;
}