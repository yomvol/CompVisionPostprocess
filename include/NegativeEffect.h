#pragma once
#include "Effect.h"

namespace gl_cv_app
{
    class NegativeEffect : public Effect
    {
    public:
        NegativeEffect() {};
        ~NegativeEffect() {};
        cv::Mat applyEffect(const cv::Mat &frame) override
        {
            cv::Mat result = frame;
            cv::bitwise_not(frame, result);
            return result;
        }

    private:


    };
}