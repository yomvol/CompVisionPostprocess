#include "Application.h"
#include "Shader.h"
#include <filesystem>

namespace gl_cv_app {

    Application::Application() : m_renderer(m_window)
    {
        if (!init())
        {
            cleanup();
            return;
        }
    }

    int Application::init()
    {
    #ifdef _DEBUG
        glfwSetErrorCallback(glfw_error_callback);
        printf("\n\nInitializing\n");
    #endif
        if (!glfwInit())
            return 0;

        auto path = std::filesystem::current_path(); //getting path
        std::filesystem::current_path(path.append("..\\..\\..\\..")); //setting path
        //path = std::filesystem::current_path();
        //std::cout << path << std::endl;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

        // Create window with graphics context
        m_window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
        if (m_window == nullptr)
            return 0;
        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1); // Enable vsync
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

        return 1;
    }

    Application::~Application()
    {
        cleanup();
    }

    void Application::run()
    {
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

            m_renderer.render(shader);

            //// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            //if (m_show_demo_window)
            //    ImGui::ShowDemoWindow(&m_show_demo_window);

            //static float f = 0.0f;
            //static int counter = 0;

            //ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.
            //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            //ImGui::Checkbox("Demo Window", &m_show_demo_window);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &m_show_another_window);

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ////ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            //if (ImGui::Button("Button"))     // Buttons return true when clicked (most widgets return true when edited/activated)
            //    counter++;
            //ImGui::SameLine();
            //ImGui::Text("counter = %d", counter);

            ////ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            //ImGui::End();

            //if (m_show_another_window)
            //{
            //    ImGui::Begin("Another Window", &m_show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            //    ImGui::Text("Hello from another window!");
            //    if (ImGui::Button("Close Me"))
            //        m_show_another_window = false;
            //    ImGui::End();
            //}

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