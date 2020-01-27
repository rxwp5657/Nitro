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

int main(int argc, char **argv)
{  
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE,  GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Nitro", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    nitro::graphics::Shader shader{"nitro.vert", "nitro.frag"};

    if(shader.Status().status_code != nitro::utils::StatusCode::OK)
    {
        std::cout << shader.Status().message << "\n";
    }

    /*
    nitro::graphics::Vertex v1{clutch::Vec3<float>{0.5f, 0.5f, 0.0},
                               clutch::Vec3<float>{},
                               clutch::Vec2<float>{1.0f, 1.0f}};
    
    nitro::graphics::Vertex v2{clutch::Vec3<float>{0.5f, -0.5f, 0.0},
                               clutch::Vec3<float>{},
                               clutch::Vec2<float>{1.0f, 0.0f}};

    nitro::graphics::Vertex v3{clutch::Vec3<float>{-0.5f, -0.5f, 0.0},
                               clutch::Vec3<float>{},
                               clutch::Vec2<float>{0.0f, 0.0f}};
    
    nitro::graphics::Vertex v4{clutch::Vec3<float>{-0.5f,  0.5f, 0.0},
                               clutch::Vec3<float>{},
                               clutch::Vec2<float>{0.0f, 1.0f}};
    
    nitro::graphics::Texture t1{"container.jpg", "../resources/models",   GL_TEXTURE0};
    nitro::graphics::Texture t2{"awesomeface.png", "../resources/models", GL_TEXTURE1, "normal", 1};

    std::vector<nitro::graphics::Vertex> pos{v1, v2, v3, v4};
    std::vector<unsigned int> indices{ 0, 1, 3, 1, 2, 3};
    std::vector<nitro::graphics::Texture> textures{t1,t2};

    nitro::graphics::Mesh mesh{pos,indices,textures};
    */

    nitro::graphics::Model model{"monkey/monkey.obj"};

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        model.Draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    }

    glfwTerminate();
    return 0;
}