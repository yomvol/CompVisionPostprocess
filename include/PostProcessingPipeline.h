#pragma once
#include "WebcamCapture.h"
#include "Effect.h"

namespace gl_cv_app {

    /// <summary>
    /// A class that is responsible for post-processing in the render pipeline.
    /// It takes input from the class WebcameraCapture and applies various effects
    /// and passes a texture to the application further.
    /// </summary>
    class PostProcessingPipeline
    {
    public:
        PostProcessingPipeline();
        ~PostProcessingPipeline();
        GLuint getTexture(cv::Mat);
        void addEffect(std::shared_ptr<Effect> effect);

    private:
        WebcamCapture m_webcam;
        GLuint m_texture;
        std::vector<std::shared_ptr<Effect>> m_effects;
    };

}