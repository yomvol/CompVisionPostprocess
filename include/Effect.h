#pragma once
#include <opencv2/opencv.hpp>

namespace gl_cv_app {

    // planned effects: negative, contours, channel filter, edge detection, sharpen, grayscale, white balance,
    // chromatic abberation, blur

    class Effect
    {
    public:
        typedef unsigned int uint;

        Effect(uint id, uint priority) : m_id(id), m_priority(priority) {};
        virtual ~Effect() { std::cout << "Effect destroyed!\n"; };
        virtual cv::Mat applyEffect(cv::Mat frame) = 0;
        static void showDebugWindow(const cv::Mat frame) { cv::imshow("Effect debug", frame); };
        inline uint getPriority() { return m_priority; };
        inline uint getID() { return m_id; };

    protected:
        uint m_priority; // effects with higher priority are applied first
        uint m_id;
    };

}