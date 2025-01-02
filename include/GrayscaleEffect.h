#pragma once
#include "Effect.h"
#include "Shader.h"

namespace gl_cv_app
{
    class GrayscaleEffect : public Effect
    {
    public:
        GrayscaleEffect(uint priority = 100) : Effect(priority) {};
        ~GrayscaleEffect() {};
        inline static uint getID() { return m_id; };

        cv::Mat applyEffect(const cv::Mat& frame) override
        {
            cv::Mat result = frame;
            cv::cvtColor(frame, result, cv::COLOR_BGR2GRAY);
            return result;
        }

        bool isUsingCustomShader() override { return true; };
        std::string getShaderPath() override { return "res\\shaders\\Grayscale.shader"; };

    private:
        static uint m_id;
    };
}