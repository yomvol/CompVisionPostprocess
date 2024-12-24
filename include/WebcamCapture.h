#pragma once
#include <opencv2/opencv.hpp>

namespace gl_cv_app {
    class WebcamCapture
    {
    public:
        WebcamCapture();
        ~WebcamCapture();
        cv::Mat getFrame();

    private:
        cv::VideoCapture m_capture;
        
    };
}