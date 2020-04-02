#define  GLEW_STATIC
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <g_manager.hpp>
#include <transforms.hpp>
#include <controller.hpp>
#include <math.h>
#include <sphere.hpp>
#include <cube.hpp>
#include <plane.hpp>
#include <skybox.hpp>

int main(int argc, char **argv)
{
    nitro::graphics::Manager manager{};
    manager.AddShader("skybox", nitro::graphics::Shader{"skybox.vert","skybox.frag"});
    
    std::shared_ptr<nitro::core::Actor> cylinder{new nitro::core::Actor{"cylinder2/cylinder.obj"}};
    std::shared_ptr<nitro::core::Actor> sphere{new nitro::core::Sphere{1.0f}};
    std::shared_ptr<nitro::core::Actor> cube{new nitro::core::Cube{1.0f}};
    std::shared_ptr<nitro::core::PointLight> light{new nitro::core::PointLight{}};
    std::shared_ptr<nitro::core::SpotLight>  spot{new nitro::core::SpotLight{}};
    std::shared_ptr<nitro::core::DirectionalLight>  dir{new nitro::core::DirectionalLight{}};

    nitro::core::Skybox skybox{"SwedishRoyalCastle", 
    {"posx.jpg",
     "negx.jpg",
     "posy.jpg",
     "negy.jpg",
     "posz.jpg",
     "negz.jpg"}};

    sphere->Translate(0.0f, 0.0f, -2.5f);
    cube->Translate(0.0f, 0.0f, 3.5f);

    spot->MovePos(0.0f, 1.0f, 1.5f);
    dir->Move(0.0f, 1.0f, 1.5f);
    light->Move(0.0f, 1.5f, 1.5f);

    cylinder->FlipUV();

    nitro::core::Scene scene{};
    nitro::input::Controller controller1{scene.CameraPtr()};

    scene.AddActor(cylinder);
    scene.AddActor(sphere);
    scene.AddActor(cube);
    //scene.AddPointLight(light);
    scene.AddSpotLight(spot);
    scene.AddDirectionalLight(dir);
    scene.AddSkyBox(skybox);

    controller1.AddButton(GLFW_KEY_W, scene.CameraPtr(), &nitro::core::Camera::Forward);
    controller1.AddButton(GLFW_KEY_A, scene.CameraPtr(), &nitro::core::Camera::Left);
    controller1.AddButton(GLFW_KEY_S, scene.CameraPtr(), &nitro::core::Camera::Backward);
    controller1.AddButton(GLFW_KEY_D, scene.CameraPtr(), &nitro::core::Camera::Right);

    float current_time = 0.0f;
    float delta_time   = 0.0f;
    float last_frame   = 0.0f;

    while(!glfwWindowShouldClose(manager.get_window()->get_window_ptr()))
    { 
        current_time = glfwGetTime();
        delta_time =  current_time - last_frame;
        last_frame = current_time;

        controller1.HandleInput(*manager.get_window(),delta_time);
        manager.UpdateScene(scene);
        
        if (glfwGetKey(manager.get_window()->get_window_ptr(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(manager.get_window()->get_window_ptr(), GL_TRUE);
    }

    glfwTerminate();
    return 0;
}