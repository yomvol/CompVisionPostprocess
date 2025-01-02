#pragma once
#include "Effect.h"

namespace gl_cv_app
{
    class BlurEffect : public Effect
    {
    public:
        BlurEffect(int kernel_size, double sigma = 0, uint priority = 90) : Effect(priority), m_kernel_size(kernel_size), m_sigma(sigma) {};
        ~BlurEffect() {};
        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result = frame;
            GaussianBlur(frame, result, cv::Size(m_kernel_size, m_kernel_size), m_sigma, 0, cv::BORDER_DEFAULT);
            return result;
        }
        bool isUsingCustomShader() override { return false; };
        void setKernelSize(int kernel_size) { m_kernel_size = kernel_size; };
        void setSigma(double sigma) { m_sigma = sigma; };

        inline static uint getID() { return m_id; };

    private:
        static uint m_id;
        int m_kernel_size = 3; // it must always be odd and positive
        double m_sigma = 0;
    };
}