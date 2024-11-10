#include "Renderer.h"

namespace gl_cv_app {

    Renderer::Renderer(GLFWwindow* window) : m_window(window), m_texture(0) {
        glfwGetFramebufferSize(m_window, &m_width, &m_height);

    }

    Renderer::~Renderer()
    {
        glDeleteTextures(1, &m_texture);
    }

    GLuint g_sh;

    void Renderer::render(Shader &shader)
    {
        GLint prevTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture); // we have to save ImGui's texture binding

        glViewport(0, 0, m_width, m_height);
        glClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w,
            m_clear_color.z * m_clear_color.w, m_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        g_sh = shader.getID();
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);

        glActiveTexture(GL_TEXTURE0);
        this->renderTexture();
        mainCanvas();

        /*GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }*/

        // Render UI
        glBindTexture(GL_TEXTURE_2D, prevTexture);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Renderer::mainCanvas()
    {
        ImGui::Begin("FX", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        ImVec2 size(1000.0f, 600.0f);
        ImGui::InvisibleButton("canvas", size);
        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 p1 = ImGui::GetItemRectMax();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        //draw_list->PushClipRect(p0, p1);
        draw_list->AddCallback([](const ImDrawList*, const ImDrawCmd*)
            {
                ImDrawData* draw_data = ImGui::GetDrawData();
                float L = draw_data->DisplayPos.x;
                float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
                float T = draw_data->DisplayPos.y;
                float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

                const float ortho_projection[4][4] =
                {
                   { 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
                   { 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
                   { 0.0f,         0.0f,        -1.0f,   0.0f },
                   { (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
                };

                //glUseProgram(g_sh);
                //glUseProgram(g_ShaderProgram);
                //glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "ProjMtx"), 1, GL_FALSE, &ortho_projection[0][0]);
            }, nullptr);
        //draw_list->PushTextureID(ImTextureID(m_texture));
        draw_list->AddImage(ImTextureID(m_texture), p0, p1, ImVec2(0, 0), ImVec2(1, 1));   
        //draw_list->PopTextureID();
        draw_list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
        //draw_list->PopClipRect();

        ImGui::End();
    }

    void Renderer::renderTexture()
    {
        m_texture = m_webcam.getTexture();
        if (m_texture == 0)
            return;

        
    }

    void GLAPIENTRY errorOccurredGL(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
        const void* userParam)
    {
        std::cout << "Message from OpenGL:" << std::endl << "Source: " << source << "\nType: " << type << "\nID: " << id
            << "\nSeverity: " << severity << std::endl;
        std::string msg(message, length);
        std::cout << msg << std::endl;
        exit(-1);
    }
}
