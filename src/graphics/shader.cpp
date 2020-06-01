#include "../../include/graphics/shader.hpp"

namespace nitro
{
    namespace graphics
    {
        Shader::Shader(const std::string& vertex, 
                       const std::string& fragment,
                       const std::string& geometry)
        {
            int success;
            status_ = utils::StatusInfo{utils::StatusCode::OK, "Shaders Compiled and Linked"};

            program_ = glCreateProgram();

            GLuint vertex_sh   = AddShader(vertex,   GL_VERTEX_SHADER);
            GLuint fragment_sh = AddShader(fragment, GL_FRAGMENT_SHADER);
            GLuint geometry_sh = 0;

            if(geometry != "")
            {
                geometry_sh = AddShader(geometry, GL_GEOMETRY_SHADER);
            }

            glLinkProgram(program_);
            glGetProgramiv(program_, GL_LINK_STATUS, &success);

            if(!success) 
            {
                status_.status_code = utils::StatusCode::BAD_LINKAGE;
                glGetShaderInfoLog(program_, 512, NULL, status_.message);
            }

            glDeleteShader(vertex_sh);
            glDeleteShader(fragment_sh);
            
            if(geometry != "")
            {
               glDeleteShader(geometry_sh);
            }

            SetUniformBlock("Lights",     1);
            SetUniformBlock("Num_Lights", 2);
        }

        Shader::Shader(const Shader& other)
        : program_{other.program_},
          uniform_dict_{other.uniform_dict_},
          status_{other.status_}
        {

        }

        Shader::Shader()
        {
              
        }

        Shader::~Shader()
        {
            
        }

        void Shader::Delete()
        {
            glDeleteProgram(program_);
        }

        GLint Shader::PosAttrib(const char *name, 
                                const GLint length,
                                const GLenum type,
                                const unsigned int size,
                                const unsigned int offset) const
        {
            GLint posAttrib = glGetAttribLocation(program_, name);
            glVertexAttribPointer(posAttrib, length, type, GL_FALSE, size, (void*) offset);
            glEnableVertexAttribArray(posAttrib);
            return posAttrib;
        }

        void Shader::SetUniformInt(const std::string& name,  const GLint val)  const
        {
            glUniform1i(GetUniformLocation(name), val);
        }

        void Shader::SetUniformFloat(const std::string& name,const GLfloat val) const
        {
            glUniform1f(GetUniformLocation(name), val);
        }

        void Shader::SetUniform2f(const std::string& name, const clutch::Vec2<float>& val) const
        {
            glUniform2fv(GetUniformLocation(name), 1, &val.x);
        }

        void Shader::SetUniform3f(const std::string& name,   const clutch::Vec3<float>& val) const
        {
            glUniform3fv(GetUniformLocation(name), 1, &val.x);
        }

        void Shader::SetUniform4f(const std::string& name,   const clutch::Vec4<float>& val) const
        {
            glUniform4fv(GetUniformLocation(name), 1, &val.x);
        }

        void Shader::SetUniformMat2(const std::string& name, const clutch::Mat2<float>& val) const
        {
            glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, clutch::ValuePtr(val));
        }

        void Shader::SetUniformMat3(const std::string& name, const clutch::Mat3<float>& val) const
        {
            glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, clutch::ValuePtr(val));
        }

        void Shader::SetUniformMat4(const std::string& name, const clutch::Mat4<float>& val) const
        {
            glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, clutch::ValuePtr(val));
        }

        void Shader::SetUniformBlock(const std::string& name, int index) const
        {
            unsigned int block_index = glGetUniformBlockIndex(program_, name.c_str());
            glUniformBlockBinding(program_, block_index, index);
        }

        GLint Shader::GetUniformLocation(const std::string& name) const
        {
            if(uniform_dict_.find(name) != uniform_dict_.end())
                return uniform_dict_[name];

            auto location = glGetUniformLocation(program_, name.c_str());
            uniform_dict_[name] = location;

            return location;
        }

        utils::StatusInfo Shader::Status() const
        {
            return status_;
        }
        
        void Shader::Use() const
        {
            glUseProgram(program_);
        }

        std::string Shader::ShaderSource(const std::string path)
        {
            try
            {
                std::string source;

                auto file = utils::Open(path);
                
                file.seekg(0, std::ios::end);   
                source.reserve(file.tellg());
                file.seekg(0, std::ios::beg);

                source.assign((std::istreambuf_iterator<char>(file)),
                std::istreambuf_iterator<char>());

                return source;
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                return "";
            }
        }

        GLuint Shader::CompileShader(const std::string file, const GLint type)
        {
            std::string rsc_path{"shaders/"};
            std::string path = rsc_path.append(file);
            std::string source = ShaderSource(path);

            char const *shader_source = source.c_str();

            GLuint shader = glCreateShader(type);

            glShaderSource(shader, 1, &shader_source, NULL);
            glCompileShader(shader);

            return shader;
        }

        GLuint Shader::AddShader(const std::string file, const GLint type)
        {
            int success;
            GLuint shader = CompileShader(file, type);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            
            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_SHADER;
                glGetShaderInfoLog(shader, 512, NULL, status_.message);
            }

            glAttachShader(program_, shader);
            return shader;
        }
    }
}