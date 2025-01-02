#pragma once
#include "Effect.h"

namespace gl_cv_app
{
    class NegativeEffect : public Effect
    {
    public:
        NegativeEffect(uint priority = 0) : Effect(priority) {};
        ~NegativeEffect() {};
        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result = frame;
            cv::bitwise_not(frame, result);
            return result;
        }
        bool isUsingCustomShader() override { return false; };

        inline static uint getID() { return m_id; };

    private:
        static uint m_id;

    };
}