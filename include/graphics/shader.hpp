#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <utils.hpp>
#include <shader.hpp>
#include <vec3.hpp>
#include <vec2.hpp>
#include <mat2.hpp>
#include <mat3.hpp>
#include <mat4.hpp>

namespace nitro
{
    namespace graphics
    {
        class Shader
        {
        public:

            Shader(const std::string& vertex, 
                   const std::string& fragment,
                   const std::string& geometry = std::string{""});
            Shader();
            Shader(const Shader& other);

            ~Shader();
            
            void Delete();

            GLint PosAttrib(const char *name, 
                            const GLint length,
                            const GLenum type,
                            const unsigned int size, 
                            const unsigned int offset) const;
            
            void SetUniformInt(const std::string& name,  const GLint val) const;
            void SetUniformFloat(const std::string& name,const GLfloat val) const;
            void SetUniform2f(const std::string& name,   const clutch::Vec2<float>& val) const;
            void SetUniform3f(const std::string& name,   const clutch::Vec3<float>& val) const;
            void SetUniform4f(const std::string& name,   const clutch::Vec4<float>& val) const;
            void SetUniformMat2(const std::string& name, const clutch::Mat2<float>& val) const;
            void SetUniformMat3(const std::string& name, const clutch::Mat3<float>& val) const;
            void SetUniformMat4(const std::string& name, const clutch::Mat4<float>& val) const;
            void SetUniformBlock(const std::string& name, int index) const;

            utils::StatusInfo Status() const;
            void Use() const;

        private:
            GLint program_;
            mutable std::map<std::string, GLint> uniform_dict_;
            mutable utils::StatusInfo status_;

            GLint GetUniformLocation(const std::string& name) const;
            std::string ShaderSource(const std::string path);
            GLuint CompileShader(const std::string file, const GLint type);
            GLuint AddShader(const std::string file, const GLint type);
        };
    }
}

#endif