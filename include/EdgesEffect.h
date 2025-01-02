#pragma once
#include "BlurEffect.h"

namespace gl_cv_app
{
    class EdgesEffect : public Effect
    {
#define EDGES_PRIORITY 0

    public:
        EdgesEffect(double th1, double th2, uint priority = EDGES_PRIORITY) : Effect(priority), m_threshold1(th1), m_threshold2(th2) {};
        ~EdgesEffect() {};
        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result = frame;
            m_blur.setKernelSize(3);
            m_blur.setSigma(0);
            result = m_blur.applyEffect(frame);
            cv::Canny(frame, result, m_threshold1, m_threshold2, 3, false);
            return result;
        }
        bool isUsingCustomShader() override { return false; };
        void setThresholds(double th1, double th2) { m_threshold1 = th1; m_threshold2 = th2; };

        inline static uint getID() { return m_id; };

    private:
        static uint m_id;
        double m_threshold1 = 100;
        double m_threshold2 = 200;
        BlurEffect m_blur = BlurEffect(0, 0, EDGES_PRIORITY + 10);

    };
}