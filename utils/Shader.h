#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>

namespace gl_utils {
    struct ShaderProgramSource
    {
        std::string vertex_source;
        std::string fragment_source;
    };

    class Shader
    {
    private:
        unsigned int m_renderer_id;
        std::string m_file_path;
        std::unordered_map<std::string, int> m_uniform_location_cache;
        int getUniformLocation(const std::string& name);
        unsigned int compileShader(unsigned int type, const std::string& source);
        unsigned int createShader(const std::string& vertex_shader, const std::string& fragment_shader);
        ShaderProgramSource parseShader();

    public:
        Shader(const std::string& filepath);
        ~Shader();

        void bind() const;
        void unbind() const;

        GLuint getID() const { return m_renderer_id; }

        // Set uniforms
        void setUniform4F(const std::string& name, float v0, float v1, float v2, float v3);
        void setUniform1F(const std::string& name, float value);
        void setUniform1I(const std::string& name, int value);
        void setUniform2F(const std::string& name, float v0, float v1);
        void setUniform3F(const std::string& name, float v0, float v1, float v2);
    };

}