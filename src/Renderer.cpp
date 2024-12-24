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

        // Render UI here
        renderUI();

        ImGui::Render();

        // we render on our framebuffer here
        m_framebuffer->Bind();
        glViewport(0, 0, m_tex_size.first, m_tex_size.second);
        shader.Bind();
        glBindTexture(GL_TEXTURE_2D, m_texture);
        shader.SetUniform1i("u_Texture", 0);
        glActiveTexture(GL_TEXTURE0);
        //this->draw(va, ib, shader);

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

    void Renderer::renderUI()
    {
        ImGui::SetNextWindowSize(ImVec2(1280, 960)); // 4:3 resolution of the webcam, my monitor is 16:9
        ImGui::Begin("Viewport", nullptr, m_viewport_flags);
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
        ImGui::End();

        ImGui::Begin("Settings", nullptr, ImGuiWindowFlags_NoCollapse);

        static bool negative = false;
        if (ImGui::Checkbox("Negative", &negative)) // if pressed
        {
            triggerEvent("OnNegativeChanged", negative);
        }
        
        if (ImGui::CollapsingHeader("Placeholder 1", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("Some content %d", i);
        }

        if (ImGui::CollapsingHeader("Placeholder 2", ImGuiTreeNodeFlags_None))
        {
            ImGui::Text("IsItemHovered: %d", ImGui::IsItemHovered());
            for (int i = 0; i < 5; i++)
                ImGui::Text("Some content %d", i);
        }

        ImGui::End();
    }

    void Renderer::registerEvent(const std::string& name, std::function<void()> handler)
    {
        m_events[name] += std::move(static_cast<std::function<void(bool)>>(utils::convert(handler)));
    }

    void Renderer::registerEvent(const std::string& name, std::function<void(bool)> handler)
    {
        m_events[name] += std::move(handler);
    }

    void Renderer::triggerEvent(const std::string& name, bool flag)
    {
        m_events[name](flag);
    }

    /*GLenum error;
        while ((error = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << error << std::endl;
        }*/

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
