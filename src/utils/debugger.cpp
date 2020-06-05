#include "../../include/utils/debugger.hpp"

namespace nitro
{
    namespace utils
    {
        Debugger::Debugger()
        : quad_vao_{0},
          quad_vbo_{0}
        {

        }

        void Debugger::DebugFrameBuffer(const unsigned int frame_buffer, const graphics::Shader& shader)
        {
            shader.Use();
            glActiveTexture(GL_TEXTURE0);  	
            glBindTexture(GL_TEXTURE_2D, frame_buffer);
            RenderQuad();
            glUseProgram(0);
        }

        void Debugger::RenderQuad()
        {
            if (quad_vao_ == 0)
            {
                float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
                };
                // setup plane VAO
                glGenVertexArrays(1, &quad_vao_);
                glGenBuffers(1, &quad_vbo_);
                glBindVertexArray(quad_vao_);
                glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_);
                glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            }
            glBindVertexArray(quad_vbo_);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }
}