#pragma once
#include <string>
#include <unordered_map>
#include <GL/glew.h>

namespace gl_cv_app {
    struct ShaderProgramSource
    {
        std::string VertexSource;
        std::string FragmentSource;
    };

    class Shader
    {
    private:
        unsigned int m_RendererID;
        std::string m_FilePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;
        int GetUniformLocation(const std::string& name);
        unsigned int CompileShader(unsigned int type, const std::string& source);
        unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
        ShaderProgramSource ParseShader();

    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        GLuint getID() { return m_RendererID; }

        // Set uniforms
        void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
        void SetUniform1f(const std::string& name, float value);
        void SetUniform1i(const std::string& name, int value);
    };

}