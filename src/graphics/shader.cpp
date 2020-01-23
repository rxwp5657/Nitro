#include <shader.hpp>

namespace nitro
{
    namespace graphics
    {
        Shader::Shader(const std::string& vertex, 
                       const std::string& fragment)
        {
            int success;
            status_ = utils::StatusInfo{utils::StatusCode::OK, "Shaders Compiled and Linked"};

            program_ = glCreateProgram();

            GLuint vertex_sh = CompileShader(vertex, GL_VERTEX_SHADER);
            glGetShaderiv(vertex_sh, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_VERTEX_SHADER;
                glGetShaderInfoLog(vertex_sh, 512, NULL, status_.message);
            }   

            GLuint fragment_sh = CompileShader(fragment, GL_FRAGMENT_SHADER);
            glGetShaderiv(fragment_sh, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_FRAGMENT_SHADER;
                glGetShaderInfoLog(fragment_sh, 512, NULL, status_.message);
            }

            glAttachShader(program_, vertex_sh);
            glAttachShader(program_, fragment_sh);
            glLinkProgram(program_);
            glGetProgramiv(program_, GL_LINK_STATUS, &success);

            if(!success) 
            {
                status_.status_code = utils::StatusCode::BAD_LINKAGE;
                glGetShaderInfoLog(program_, 512, NULL, status_.message);
            }

            glDeleteShader(vertex_sh);
            glDeleteShader(fragment_sh);
        }

        Shader::Shader(const std::string& vertex, 
                           const std::string& fragment,
                           const std::string& geometry)
        {
            int success;
            status_ = utils::StatusInfo{utils::StatusCode::OK, "Shaders Compiled and Linked"};

            program_ = glCreateProgram();

            GLuint vertex_sh = CompileShader(vertex, GL_VERTEX_SHADER);
            glGetShaderiv(vertex_sh, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_VERTEX_SHADER;
                glGetShaderInfoLog(vertex_sh, 512, NULL, status_.message);
            }   

            GLuint fragment_sh = CompileShader(fragment, GL_FRAGMENT_SHADER);
            glGetShaderiv(fragment_sh, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_FRAGMENT_SHADER;
                glGetShaderInfoLog(fragment_sh, 512, NULL, status_.message);
            }

            GLuint geometry_sh = CompileShader(geometry, GL_GEOMETRY_SHADER);
            glGetShaderiv(geometry_sh, GL_COMPILE_STATUS, &success);

            if(!success)
            {
                status_.status_code = utils::StatusCode::BAD_GEOMETRY_SHADER;
                glGetShaderInfoLog(geometry_sh, 512, NULL, status_.message);
            } 

            glAttachShader(program_, vertex_sh);
            glAttachShader(program_, fragment_sh);
            glAttachShader(program_, geometry_sh);
            glLinkProgram(program_);
            glGetProgramiv(program_, GL_LINK_STATUS, &success);

            if(!success) 
            {
                status_.status_code = utils::StatusCode::BAD_LINKAGE;
                glGetShaderInfoLog(program_, 512, NULL, status_.message);
            }

            glDeleteShader(vertex_sh);
            glDeleteShader(fragment_sh);
            glDeleteShader(geometry_sh);
        }

        GLint Shader::PosAttrib(const char *name, 
                                const GLint length,
                                const GLenum type,
                                const unsigned int size,
                                const unsigned int offset) const
        {
            GLint posAttrib = glGetAttribLocation(program_, name);
            glEnableVertexAttribArray(posAttrib);
            glVertexAttribPointer(posAttrib, length, type, GL_FALSE, size, (void*) offset);
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

        void Shader::SetUniform2f(const std::string& name,   const clutch::Vec2<float>& val) const
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

        GLint Shader::GetUniformLocation(const std::string& name) const
        {
            if(uniform_dict_.find(name) != uniform_dict_.end())
                return uniform_dict_[name];

            auto location = glGetUniformLocation(program_, name.c_str());
            uniform_dict_[name] = location;

            return location;
        }

        utils::StatusInfo Shader::Status()
        {
            return status_;
        }
        
        void Shader::Use()
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
            std::string rsc_path{"../resources/shaders/"};
            std::string path = rsc_path.append(file);
            std::string source = ShaderSource(path);

            char const *shader_source = source.c_str();

            GLuint shader = glCreateShader(type);

            glShaderSource(shader, 1, &shader_source, NULL);
            glCompileShader(shader);

            return shader;
        }

        void Shader::SetTextureUniforms()
        {

        }
    }
}