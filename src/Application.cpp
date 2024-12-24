#include "Application.h"
#include <filesystem>

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
    #ifdef _DEBUG
        glfwSetErrorCallback(glfwErrorCallback);
        printf("\n\nInitializing\n");
    #endif
        if (!glfwInit())
            return false;

        auto path = std::filesystem::current_path(); //getting path
        std::filesystem::current_path(path.append("..\\..\\..\\..")); //setting path
        //path = std::filesystem::current_path();
        //std::cout << path << std::endl;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        // Create window with graphics context
        m_window = glfwCreateWindow(1600, 900, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
        if (m_window == nullptr)
            return false;
        m_renderer.init(m_window);
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);
        if (glewInit() != GLEW_OK) // initialize glew
            std::cout << "Glew init failed!" << std::endl;
        std::cout << glGetString(GL_VERSION) << std::endl;
        glEnable(GL_TEXTURE_2D);

#ifdef _DEBUG
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(errorOccurredGL, NULL);
#endif

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        //(void)io;
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
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0 bottom left
             0.5f, -0.5, 1.0f, 0.0f,  // 1 bottom right
             0.5f,  0.5f, 1.0f, 1.0f, // 2 top right
             -0.5f, 0.5f, 0.0f, 1.0f  // 3 top left
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
        va.AddBuffer(vb, layout);
        Shader shader("res\\shaders\\Basic.shader");

        // Main loop
        while (!glfwWindowShouldClose(m_window))
        {
            // Poll and handle events (inputs, window resize, etc.)
            glfwPollEvents();
            if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
            {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
            m_controller.view->setTexture(m_controller.model->getTexture());
            m_controller.view->render(va, ib, shader);

            glfwSwapBuffers(m_window);
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