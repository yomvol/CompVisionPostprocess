#include "PostProcessingPipeline.h"

gl_cv_app::PostProcessingPipeline::PostProcessingPipeline() : m_texture(-1) {}

gl_cv_app::PostProcessingPipeline::~PostProcessingPipeline()
{
    if (m_texture != -1) glDeleteTextures(1, &m_texture);
}

GLuint gl_cv_app::PostProcessingPipeline::getTexture()
{
    cv::Mat frame = m_webcam.getFrame();
    if (frame.empty())
        return -1;

    // Apply effects
    if (!m_effects.empty())
    {
        for (auto& effect : m_effects)
        {
            frame = effect->applyEffect(frame);
        }
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Setup texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload the frame to the texture frame.data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);

    return m_texture;
}

void gl_cv_app::PostProcessingPipeline::addEffect(const std::shared_ptr<Effect> effect)
{
    m_effects.insert(effect);
}

void gl_cv_app::PostProcessingPipeline::removeEffect(const std::shared_ptr<Effect> effect)
{
    m_effects.erase(effect);
}
