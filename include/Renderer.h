#pragma once
#include <GL/glew.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "WebcamCapture.h"
#include "Shader.h"

namespace gl_cv_app {
    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
        const void* userParam);

    class Renderer {
    public:
        Renderer(GLFWwindow* window);
        ~Renderer();
        void render(Shader &shader);
        void setTexture(GLuint texture) { m_texture = texture; glBindTexture(GL_TEXTURE_2D, m_texture); }
        void setIO(ImGuiIO* io) { this->io = io; }

    private:
        GLFWwindow* m_window;
        GLuint m_texture;
        int m_width;
        int m_height;
        WebcamCapture m_webcam;
        ImGuiIO* io;
        ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        void mainCanvas();
        void renderTexture();
        void renderUI();

    };
}
