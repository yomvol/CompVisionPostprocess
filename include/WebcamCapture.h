#pragma once
#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace gl_cv_app {
    class WebcamCapture
    {
    public:
        WebcamCapture();
        ~WebcamCapture();
        GLuint getTexture();

    private:
        cv::VideoCapture m_capture;
        GLuint m_texture;

        cv::Mat getFrame();

    };
}