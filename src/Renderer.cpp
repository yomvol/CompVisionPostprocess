#include "Renderer.h"
#include "Controller.h"
#include <iostream>

namespace gl_cv_app {

#pragma region UIState
    bool Renderer::UIState::isNegative = false;
    bool Renderer::UIState::isGrayscale = false;
    bool Renderer::UIState::isBlur = false;
    bool Renderer::UIState::isEdges = false;
    bool Renderer::UIState::isContours = false;
    bool Renderer::UIState::isTriangulation = false;
    bool Renderer::UIState::isDenoising = false;
    bool Renderer::UIState::isAcid = false;
#pragma endregion

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
        shader.bind();
        va.bind();
        ib.bind();

        glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
    }

    void Renderer::render()
    {
        glClearColor(m_clear_color.x * m_clear_color.w, m_clear_color.y * m_clear_color.w,
            m_clear_color.z * m_clear_color.w, m_clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render UI here
        renderUI();

        ImGui::Render();

        if (m_is_no_cam_error)
        {
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

            return;
        }

        // we render on our framebuffer here
        // do we even need to use a shader?
        std::shared_ptr<Shader> shader;
        bool is_custom_shader = controller->isUsingCustomShader(shader);
        if (is_custom_shader)
        {
            float positions[] = {
            -1.0f, -1.0f, 0.0f, 0.0f, // 0 bottom left
             1.0f, -1.0, 1.0f, 0.0f,  // 1 bottom right
             1.0f,  1.0f, 1.0f, 1.0f, // 2 top right
             -1.0f, 1.0f, 0.0f, 1.0f  // 3 top left
            };

            unsigned int indices[] = {
                0, 1, 2,
                2, 3, 0
            };

            VertexBuffer vb(positions, 4 * 4 * sizeof(float));
            IndexBuffer ib(indices, 6);
            VertexArray va;
            VertexBufferLayout layout;
            layout.Push<float>(2);
            layout.Push<float>(2);
            va.addBuffer(vb, layout);
            //Shader shader("res\\shaders\\Basic.shader");

            m_framebuffer->bind();
            glViewport(0, 0, m_tex_size.first, m_tex_size.second);
            shader->bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_texture);
            shader->setUniform1I("u_Texture", 0);

            this->draw(va, ib, *shader);

            va.unbind();
            shader->unbind();
            m_framebuffer->unbind();
        }

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

    std::pair<int, int> Renderer::getWindowSize()
    {
        glfwGetFramebufferSize(m_window, &m_width, &m_height);
        return { m_width, m_height };
    }

    void Renderer::showConflictMsg()
    {
        ImGui::OpenPopup("Conflict");
        if (ImGui::BeginPopupModal("Conflict", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text("You cannot apply this effect because it conflicts with another effect.\nPlease disable all other effects.");
            if (ImGui::Button("OK", ImVec2(120, 0))) { m_is_showing_conflict = false; ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }
    }

    void Renderer::renderUI()
    {
        if (m_is_no_cam_error)
        {
            ImGui::SetNextWindowPos(ImVec2(700, 400));
            ImGui::SetNextWindowSize(ImVec2(640, 360));
            ImGui::Begin("Error", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
            ImGui::Text("No webcam detected. Please connect a webcam and restart the application.");
            ImGui::End();
            return;
        }

        ImGui::SetNextWindowSize(ImVec2(1280, 960)); // 4:3 resolution of the webcam, my monitor is 16:9
        ImGui::Begin("Viewport", nullptr, m_viewport_flags);
        const float window_width = ImGui::GetContentRegionAvail().x;
        const float window_height = ImGui::GetContentRegionAvail().y;
        ImGui::Text("FPS: %.1f", 1.0f / m_io->DeltaTime);
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_io->Framerate, m_io->Framerate);

        if (m_is_showing_conflict) showConflictMsg();

        // rescale framebuffer and update webcam texture
        m_framebuffer->update(m_texture, m_tex_size.first, m_tex_size.second);
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

        if (ImGui::Checkbox("Negative", &UIState::isNegative)) // if pressed
        {
            events.NegativeChanged(UIState::isNegative);
        }

        if (ImGui::Checkbox("Grayscale", &UIState::isGrayscale)) // if pressed
        {
            events.GrayscaleChanged(UIState::isGrayscale);
        }
        
        if (ImGui::CollapsingHeader("Gaussian Blur", ImGuiTreeNodeFlags_None))
        {
            ImGui::PushID("Gaussian Blur");
            if (ImGui::Checkbox("Enable", &UIState::isBlur))
            {
                events.BlurChanged(UIState::isBlur, m_blur_radius);
            }
            
            if (ImGui::SliderFloat("Blur radius", &m_blur_radius, 0.0f, 10.0f))
            {
                events.BlurChanged(UIState::isBlur, m_blur_radius);
            }
            ImGui::PopID();
        }

        static float denoising_strength = 75.0f;

        if (ImGui::CollapsingHeader("Denoising", ImGuiTreeNodeFlags_None))
        {
            ImGui::PushID("Denoising");
            if (ImGui::Checkbox("Enable", &UIState::isDenoising))
            {
                events.DenoisingChanged(UIState::isDenoising, denoising_strength);
            }

            if (ImGui::SliderFloat("Filter strength", &denoising_strength, 0.0f, 200.0f))
            {
                events.DenoisingChanged(UIState::isDenoising, denoising_strength);
            }
            ImGui::SetItemTooltip("Strength of the denoising filter. Higher values mean more aggressive denoising.");
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Canny Edge Detection", ImGuiTreeNodeFlags_None))
        {
            ImGui::PushID("Canny Edge Detection");
            if (ImGui::Checkbox("Enable", &UIState::isEdges))
            {
                events.EdgesChanged(UIState::isEdges, m_canny_lower_threshold, m_canny_upper_threshold);
            }

            if (ImGui::SliderFloat("Lower threshold", &m_canny_lower_threshold, 0.0f, 100.0f))
            {
                events.EdgesChanged(UIState::isEdges, m_canny_lower_threshold, m_canny_upper_threshold);
            }
            ImGui::SetItemTooltip("If a pixel gradient value is below the lower threshold, then it is rejected.");

            if (ImGui::SliderFloat("Upper threshold", &m_canny_upper_threshold, 100.0f, 200.0f))
            {
                events.EdgesChanged(UIState::isEdges, m_canny_lower_threshold, m_canny_upper_threshold);
            }
            ImGui::SetItemTooltip("If a pixel gradient is higher than the upper threshold, the pixel is accepted as an edge.\nIf the pixel gradient is between the two thresholds, then it will be accepted only if it is connected to a pixel that is above the upper threshold.");
            ImGui::PopID();
        }

        static ImVec4 contour_color = ImVec4(0.5f, 0.0f, 1.0f, 1.0f);
        if (ImGui::CollapsingHeader("Find Contours", ImGuiTreeNodeFlags_None))
        {
            ImGui::PushID("Find Contours");
            if (ImGui::Checkbox("Enable", &UIState::isContours))
            {
                events.ContoursChanged(UIState::isContours, m_contour_threshold, contour_color, m_contour_thickness);
            }

            if (ImGui::SliderFloat("Threshold", &m_contour_threshold, 0.0f, 255.0f))
            {
                events.ContoursChanged(UIState::isContours, m_contour_threshold, contour_color, m_contour_thickness);
            }
            ImGui::SetItemTooltip("Threshold value for binary image. Pixels with intensity higher than this value will be set to 255, otherwise to 0.");

            if (ImGui::SliderInt("Thickness", &m_contour_thickness, 1, 5))
            {
                events.ContoursChanged(UIState::isContours, m_contour_threshold, contour_color, m_contour_thickness);
            }
            ImGui::SetItemTooltip("Thickness of the contour lines to draw.");

            if (ImGui::ColorEdit4("Color", (float*)&contour_color, ImGuiColorEditFlags_NoInputs))
            {
                events.ContoursChanged(UIState::isContours, m_contour_threshold, contour_color, m_contour_thickness);
            }

            ImGui::PopID();
        }

        static int is_delaunay = 1;
        static ImVec4 triangle_line_color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        static int triangle_threshold = 10;
        static bool draw_centers = true;
        if (ImGui::CollapsingHeader("Delaunay/Voronoi", ImGuiTreeNodeFlags_None))
        {
            ImGui::PushID("Triangulation");
            if (ImGui::Checkbox("Enable", &UIState::isTriangulation))
            {
                events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
            }

            if (ImGui::RadioButton("Delaunay", &is_delaunay, 1))
            {
                events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("Voronoi", &is_delaunay, 0))
            {
                events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
            }

            if (ImGui::SliderInt("Keypoint threshold", &triangle_threshold, 5, 25))
            {
                events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
            }
            ImGui::SetItemTooltip("Threshold for FAST keypoint detection algorithm. Higher threshold gives lower sensitivity.");

            if (is_delaunay)
            {
                if (ImGui::ColorEdit4("Color", (float*)&triangle_line_color, ImGuiColorEditFlags_NoInputs))
                {
                    events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
                }
            }
            else
            {
                if (ImGui::Checkbox("Draw locus centers", &draw_centers))
                {
                    events.TriangulationChanged(UIState::isTriangulation, is_delaunay, triangle_line_color, triangle_threshold, draw_centers);
                }
            }
            ImGui::PopID();
        }

        if (ImGui::CollapsingHeader("Accid effect", ImGuiTreeNodeFlags_None))
        {
            if (ImGui::Checkbox("Apply effect", &UIState::isAcid))
            {
                events.AcidChanged(UIState::isAcid);
            }
        }

        ImGui::End();
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
