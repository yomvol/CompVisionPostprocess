#pragma once
#include "WebcamCapture.h"
#include "Effect.h"
#include "Shader.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <set>

namespace gl_cv_app {

    /// <summary>
    /// Model component of the application manages business logic.
    /// A class that is responsible for post-processing in the render pipeline.
    /// It takes input from the class WebcameraCapture and applies various effects
    /// and passes a texture to the application further.
    /// </summary>
    class PostProcessingPipeline
    {
        struct EffectComparator
        {
            bool operator()(const std::shared_ptr<Effect>& lhs, const std::shared_ptr<Effect>& rhs) const
            {
                if (rhs == nullptr)
                    return true;
                if (lhs == nullptr)
                    return false;

                return lhs->getPriority() > rhs->getPriority();
            }
        };

    public:
        PostProcessingPipeline();
        ~PostProcessingPipeline();
        GLuint getTexture();
        GLuint getPrevTexture() { return m_prev_texture; };
        void addEffect(const std::shared_ptr<Effect> effect);
        void removeEffect(const std::shared_ptr<Effect> effect);
        std::shared_ptr<Shader> getShaderIfAny();
        float getTime() { return glfwGetTime(); };
        std::pair<float, float> getResolution();
        void setResolution(std::pair<int, int> res) { m_resolution = res; };

    private:
        WebcamCapture m_webcam;
        GLuint m_texture;
        GLuint m_prev_texture;
        std::set<std::shared_ptr<Effect>, EffectComparator> m_effects;
        std::pair<int, int> m_resolution;
    };

}