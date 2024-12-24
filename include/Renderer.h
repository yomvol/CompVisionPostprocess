#pragma once
#include <GL/glew.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Framebuffer.h"
#include "../utils/Event.h"

namespace gl_cv_app {
    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
        const void* userParam);

    /// <summary>
    /// View component of the application. It is responsible for rendering GUI.
    /// </summary>

    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void init(GLFWwindow* window);
        void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void render(const VertexArray& va, const IndexBuffer& ib, Shader& shader);
        void setTexture(GLuint texture, int width, int height) { m_texture = texture; m_tex_size.first = width; m_tex_size.second = height; }
        void setTexture(GLuint texture) { m_texture = texture; }
        void setIO(ImGuiIO* io) { this->m_io = io; }
        void createFramebuffer(GLuint texture);


        void registerEvent(const std::string& name, std::function<void()> handler);
        void registerEvent(const std::string& name, std::function<void(bool)> handler);

        void triggerEvent(const std::string& name, bool flag = false);

    private:
        GLFWwindow* m_window;
        GLuint m_texture;
        std::pair<int, int> m_tex_size;
        int m_width;
        int m_height;
        std::unique_ptr<Framebuffer> m_framebuffer;
        ImGuiIO* m_io;
        ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ImGuiWindowFlags m_viewport_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
             ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar;

        std::unordered_map<std::string, utils::Event<bool>> m_events;

        void renderUI();
        

    };
}
