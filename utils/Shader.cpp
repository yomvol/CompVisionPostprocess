#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>

namespace gl_utils {

    Shader::Shader(const std::string& filepath) : m_file_path(filepath), m_renderer_id(0)
    {
        ShaderProgramSource source = parseShader();
        m_renderer_id = createShader(source.vertex_source, source.fragment_source);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_renderer_id);
    }

    unsigned int Shader::compileShader(unsigned int type, const std::string& source)
    {
        unsigned int id = glCreateShader(type);
        const char* src = source.c_str();
        glShaderSource(id, 1, &src, NULL);
        glCompileShader(id);

        int result;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE)
        {
            int length;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)alloca(length * sizeof(char));
            glGetShaderInfoLog(id, length, &length, message);
            std::cout << "Failed to compile " << (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex")
                << " shader!" << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(id);
            return 0;
        }

        return id;
    }

    ShaderProgramSource Shader::parseShader()
    {
        std::fstream stream(m_file_path);

        enum class ShaderType
        {
            NONE = -1, VERTEX = 0, FRAGMENT = 1
        };

        std::string line;
        std::stringstream ss[2];
        ShaderType type = ShaderType::NONE;
        while (getline(stream, line))
        {
            if (line.find("#shader") != std::string::npos)
            {
                if (line.find("vertex") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("fragment") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            }
            else
            {
                ss[(int)type] << line << '\n';
            }
        }

        return { ss[0].str(), ss[1].str() };
    }

    unsigned int Shader::createShader(const std::string& vertex_shader, const std::string& fragment_shader)
    {
        unsigned int program = glCreateProgram();
        unsigned int vs = compileShader(GL_VERTEX_SHADER, vertex_shader);
        unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragment_shader);

        glAttachShader(program, vs);
        glAttachShader(program, fs);
        glLinkProgram(program);
        glValidateProgram(program);

        glDeleteShader(vs);
        glDeleteShader(fs);

        return program;
    }

    void Shader::bind() const
    {
        glUseProgram(m_renderer_id);
    }

    void Shader::unbind() const
    {
        glUseProgram(0);
    }

    void Shader::setUniform4F(const std::string& name, float v0, float v1, float v2, float v3)
    {
        glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
    }

    void Shader::setUniform1F(const std::string& name, float value)
    {
        glUniform1f(getUniformLocation(name), value);

    }

    void Shader::setUniform1I(const std::string& name, int value)
    {
        glUniform1i(getUniformLocation(name), value);
    }

    void Shader::setUniform2F(const std::string& name, float v0, float v1)
    {
        glUniform2f(getUniformLocation(name), v0, v1);
    }

    void Shader::setUniform3F(const std::string& name, float v0, float v1, float v2)
    {
        glUniform3f(getUniformLocation(name), v0, v1, v2);
    }

    int Shader::getUniformLocation(const std::string& name)
    {
        if (m_uniform_location_cache.find(name) != m_uniform_location_cache.end())
            return m_uniform_location_cache[name];

        int location = glGetUniformLocation(m_renderer_id, name.c_str());
        if (location == -1)
            std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
        m_uniform_location_cache[name] = location;
        return location;
    }
}