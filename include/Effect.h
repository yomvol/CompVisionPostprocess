#pragma once
#include <opencv2/opencv.hpp>

namespace gl_cv_app {

    // planned effects: negative, contours, channel filter, edge detection, sharpen, grayscale, white balance,
    // chromatic abberation, blur

    class Effect
    {
    public:
        Effect() {};
        virtual ~Effect() {};
        virtual cv::Mat applyEffect(const cv::Mat &frame) = 0;
        static void showDebugWindow(const cv::Mat &frame) { cv::imshow("Effect debug", frame); };

    protected:
        

    };

}