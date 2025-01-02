#pragma once
#include "GrayscaleEffect.h"

namespace gl_cv_app
{
    class ContourEffect : public Effect
    {
    public:
        ContourEffect(double threshold, cv::Scalar color, int thickness, uint priority = 0) :
            m_threshold(threshold), m_color(color), m_thickness(thickness), Effect(priority) {};
        ~ContourEffect() {};
        inline static uint getID() { return m_id; };
        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat gray = m_grayscale.applyEffect(frame);
            cv::Mat binary, result = frame;
            cv::threshold(gray, binary, m_threshold, 255, cv::THRESH_BINARY);
            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(binary, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
            cv::drawContours(result, contours, -1, m_color, m_thickness);
            return result;
        }
        bool isUsingCustomShader() override { return false; };
        void setThreshold(double threshold) { m_threshold = threshold; };
        void setColor(cv::Scalar color) { m_color = color; };
        void setThickness(int thickness) { m_thickness = thickness; };

    private:
        static uint m_id;
        GrayscaleEffect m_grayscale = GrayscaleEffect();
        double m_threshold = 90;
        int m_thickness = 2;
        cv::Scalar m_color = cv::Scalar(0, 255, 0);

    };
}