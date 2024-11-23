#include "Renderer.h"
#include <iostream>

namespace gl_cv_app {

    Renderer::Renderer() : m_texture(0), m_tex_size(0, 0) {}

    Renderer::~Renderer()
    {
        glDeleteTextures(1, &m_texture);
    }

    void Renderer::init(GLFWwindow* window)
    {
        m_window = window;
        glfwGetFramebufferSize(m_window, &m_width, &m_height);
    }

    void Renderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
    {
        shader.Bind();
        va.Bind();
        ib.Bind();

        glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::render(const VertexArray& va, const IndexBuffer& ib, Shader& shader)
    {
        glClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w,
            m_clear_color.z * m_clear_color.w, m_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::SetNextWindowSize(ImVec2(1280, 960)); // 4:3 resolution of the webcam, my monitor is 16:9
        ImGui::Begin("Viewport");
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;

        // rescale framebuffer and update webcam texture
        m_framebuffer->Update(m_texture, m_tex_size.first, m_tex_size.second);
        glViewport(0, 0, m_tex_size.first, m_tex_size.second);

        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddImage(
            (void*)m_texture,
            ImVec2(pos.x, pos.y),
            ImVec2(pos.x + window_width, pos.y + window_height),
            ImVec2(0, 1),
            ImVec2(1, 0)
        );

        /*GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }*/

        // Render UI here

        ImGui::End();
        ImGui::Render();

        // we render on our framebuffer here
        m_framebuffer->Bind();
        glViewport(0, 0, m_tex_size.first, m_tex_size.second);
        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, m_texture);
        shader.SetUniform1i("u_Texture", 0);
        glActiveTexture(GL_TEXTURE0);
        this->draw(va, ib, shader);

        va.Unbind();
        shader.Unbind();
        m_framebuffer->Unbind();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void Renderer::createFramebuffer(GLuint texture)
    {
        m_framebuffer = std::make_unique<Framebuffer>(texture, m_width, m_height);
    }

    //void Renderer::mainCanvas()
    //{
    //    ImGui::Begin("FX", NULL, ImGuiWindowFlags_AlwaysAutoResize);
    //    ImVec2 size(1200.0f, 800.0f);
    //    ImGui::InvisibleButton("canvas", size);
    //    ImVec2 p0 = ImGui::GetItemRectMin();
    //    ImVec2 p1 = ImGui::GetItemRectMax();
    //    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    //    //draw_list->PushClipRect(p0, p1);
    //    //draw_list->AddCallback([](const ImDrawList*, const ImDrawCmd*)
    //    //    {
    //    //        ImDrawData* draw_data = ImGui::GetDrawData();
    //    //        float L = draw_data->DisplayPos.x;
    //    //        float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    //    //        float T = draw_data->DisplayPos.y;
    //    //        float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    //    //        const float ortho_projection[4][4] =
    //    //        {
    //    //           { 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
    //    //           { 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
    //    //           { 0.0f,         0.0f,        -1.0f,   0.0f },
    //    //           { (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
    //    //        };

    //    //        //glUseProgram(g_ShaderProgram);
    //    //        //glUniformMatrix4fv(glGetUniformLocation(g_ShaderProgram, "ProjMtx"), 1, GL_FALSE, &ortho_projection[0][0]);
    //    //    }, nullptr);
    //    //draw_list->PushTextureID(ImTextureID(m_texture));
    //    draw_list->AddImage(ImTextureID(m_texture), p0, p1, ImVec2(0, 0), ImVec2(1, 1));   
    //    //draw_list->PopTextureID();
    //    draw_list->AddCallback(ImDrawCallback_ResetRenderState, nullptr);
    //    //draw_list->PopClipRect();

    //    ImGui::End();
    //}

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
