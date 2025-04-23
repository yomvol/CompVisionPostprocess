#pragma once
#include <opencv2/opencv.hpp>
#include "Shader.h"

namespace gl_cv_app {

    // planned effects: negative, contours, channel filter, edge detection, sharpen, grayscale, white balance,
    // chromatic abberation, blur

    class PostProcessingPipeline;

    class Effect
    {
    public:
        typedef unsigned int uint;

        Effect(uint priority) : m_priority(priority) {};
        virtual ~Effect() { std::cout << "Effect destroyed!\n"; };
        virtual cv::Mat applyEffect(const cv::Mat& frame) = 0;
        static void showDebugWindow(const cv::Mat& frame) { cv::imshow("Effect debug", frame); };
        inline uint getPriority() { return m_priority; };
        // inline static uint getID() { return m_id; }; ALWAYS DEFINE THIS GETTER IN DERIVED CLASSES

        virtual bool isUsingCustomShader() = 0;
        virtual void setUniforms(std::shared_ptr<gl_utils::Shader> shader_ptr, PostProcessingPipeline* pipe) {};
        virtual std::string getShaderPath() { return ""; };

    protected:
        uint m_priority; // effects with higher priority are applied first
        static const uint m_id = 0; // ALWAYS REDEFINE THIS IN DERIVED CLASSES
    };

}