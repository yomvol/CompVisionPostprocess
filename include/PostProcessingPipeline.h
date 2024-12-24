#pragma once
#include "WebcamCapture.h"
#include "Effect.h"
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
                return lhs->getPriority() > rhs->getPriority();
            }
        };

    public:
        PostProcessingPipeline();
        ~PostProcessingPipeline();
        GLuint getTexture();
        void addEffect(const std::shared_ptr<Effect> effect);
        void removeEffect(const std::shared_ptr<Effect> effect);

    private:
        WebcamCapture m_webcam;
        GLuint m_texture;
        std::set<std::shared_ptr<Effect>, EffectComparator> m_effects;
    };

}