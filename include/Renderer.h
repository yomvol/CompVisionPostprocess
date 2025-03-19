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

    class Controller;

    /// <summary>
    /// View component of the application. It is responsible for rendering GUI.
    /// </summary>

    class Renderer {
    public:
        Renderer();
        ~Renderer();
        void init(GLFWwindow* window);
        void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
        void render();
        void setTexture(GLuint texture, int width, int height) { m_texture = texture; m_tex_size.first = width; m_tex_size.second = height; }
        void setTexture(GLuint texture) { m_texture = texture; }
        void setIO(ImGuiIO* io) { this->m_io = io; }
        void createFramebuffer(GLuint texture);
        std::pair<int, int> getWindowSize();
        void setIsShowingConflict(bool flag) { m_is_showing_conflict = flag; }
        void setNoCamError() { m_is_no_cam_error = true; }
        bool getNoCamError() { return m_is_no_cam_error; }

        typedef struct Events {
            utils::Event<bool> NegativeChanged;
            utils::Event<bool> GrayscaleChanged;
            utils::Event<bool, float> BlurChanged;
            utils::Event<bool, float, float> EdgesChanged;
            utils::Event<bool, float, ImVec4, int> ContoursChanged;
            utils::Event<bool, bool, ImVec4, int, bool> TriangulationChanged;
            utils::Event<bool, float> DenoisingChanged;
            utils::Event<bool> AcidChanged;
        } Events;
        Events events;

        Controller* controller;

        class UIState
        {
        public:
            UIState() = delete;
            static bool isNegative;
            static bool isGrayscale;
            static bool isBlur;
            static bool isEdges;
            static bool isContours;
            static bool isTriangulation;
            static bool isDenoising;
            static bool isAcid;
        };

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

        float m_blur_radius = 0;
        float m_canny_lower_threshold = 50;
        float m_canny_upper_threshold = 200;
        float m_contour_threshold = 0;
        int m_contour_thickness = 1;
        
        bool m_is_showing_conflict = false;
        bool m_is_no_cam_error = false;

        void renderUI();
        void showConflictMsg();
    };
}
