#pragma once
#ifdef _DEBUG
#include <stdio.h>

static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
#endif

#include "Renderer.h"
#include "PostProcessingPipeline.h"
#include "Controller.h"

extern const unsigned int Roboto_Medium_compressed_size;
extern const unsigned int Roboto_Medium_compressed_data[];

namespace gl_cv_app {

    class Application
    {
    public:
        Application();
        ~Application();
        void run();

    private:
        GLFWwindow* m_window;
        Renderer m_renderer;
        PostProcessingPipeline m_pipe;
        Controller m_controller;

        bool init();
        void cleanup();

        bool m_should_close = false;
    };
}