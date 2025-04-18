#include "Application.h"
#include <filesystem>
#include <thread>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace gl_cv_app {

    Application::Application() : m_controller(&m_renderer, &m_pipe)
    {
        if (!init())
        {
            cleanup();
            return;
        }
    }

    bool Application::init()
    {
#ifdef DEBUG_BUILD
        glfwSetErrorCallback(glfwErrorCallback);
        printf("\n\nInitializing\n");
    #endif
        if (!glfwInit())
            return false;

        auto path = std::filesystem::current_path(); //getting path
        
#ifdef DEBUG_BUILD
        std::filesystem::current_path(path.append(R"(..\..\..\..)")); //setting path
#endif

        //path = std::filesystem::current_path();
        //std::cout << path << std::endl;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef DEBUG_BUILD
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        // Create window with graphics context
        m_window = glfwCreateWindow(1600, 900, "Computer Vision effects explorer", nullptr, nullptr);
        if (m_window == nullptr)
            return false;
        m_renderer.init(m_window);
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
        if (glewInit() != GLEW_OK) // initialize glew
            std::cout << "Glew init failed!" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;
        glEnable(GL_TEXTURE_2D);

#ifdef DEBUG_BUILD
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(errorOccurredGL, NULL);
#endif

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
        io.ConfigViewportsNoTaskBarIcon = true;
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(m_window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF((const void*)Roboto_Medium_compressed_data, Roboto_Medium_compressed_size, 20);
        m_renderer.setIO(&io);

        if (!m_pipe.getIsAliveAndWell())
        {
            return true;
        }

        GLuint texture = m_controller.model->getTexture();
        int w, h;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
        m_controller.view->setTexture(texture, w, h);
        m_controller.view->createFramebuffer(texture);

        return true;
    }

    Application::~Application()
    {
        cleanup();
    }

    void Application::run()
    {
        // Main loop
        while (!glfwWindowShouldClose(m_window) && !m_should_close)
        {
            // Poll and handle events (inputs, window resize, etc.)
            glfwPollEvents();
            if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            if (!m_pipe.getIsAliveAndWell())
            {
                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

                if (!m_renderer.getNoCamError())
                {
                    std::chrono::seconds delay(5);
                    auto destroyTimerHandler = [this]() { m_should_close = true; };

                    std::thread([delay, destroyTimerHandler]() {
                        std::this_thread::sleep_for(delay);
                        destroyTimerHandler();
                        }).detach();
                }

                m_renderer.setNoCamError();
                m_renderer.render();
                glfwSwapBuffers(m_window);
            }
            else
            {
                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

                m_controller.view->setTexture(m_controller.model->getTexture());
                m_controller.view->render();

                glfwSwapBuffers(m_window);
            }
        }
    }

    void Application::cleanup()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        glfwTerminate();
    }

}