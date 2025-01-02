#pragma once
#include "Effect.h"

namespace gl_cv_app
{
    class DenoisingEffect : public Effect
    {
    public:
        DenoisingEffect(float strength, uint priority = 120) : m_filter_strength(strength), Effect(priority) {};
        ~DenoisingEffect() {};
        inline static uint getID() { return m_id; };

        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result;
            cv::bilateralFilter(frame, result, 10, m_filter_strength, m_filter_strength);
            return result;
        }
        bool isUsingCustomShader() override { return false; };
        void setFilterStrength(float filter_strength) { m_filter_strength = filter_strength; };

    private:
        static uint m_id;
        float m_filter_strength = 75;
    };
}