#pragma once
#ifdef _DEBUG
#include <stdio.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}
#endif

#include "Renderer.h"
#include "WebcamCapture.h"

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
        bool m_show_demo_window = true;
        bool m_show_another_window = false;
        WebcamCapture m_webcam;

        int init();
        void cleanup();


    };
}