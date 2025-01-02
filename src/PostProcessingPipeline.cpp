#include "PostProcessingPipeline.h"
#include "Shader.h"

gl_cv_app::PostProcessingPipeline::PostProcessingPipeline() : m_texture(-1), m_prev_texture(-1) {}

gl_cv_app::PostProcessingPipeline::~PostProcessingPipeline()
{
    if (m_texture != -1) glDeleteTextures(1, &m_texture);
    if (m_prev_texture != -1) glDeleteTextures(1, &m_prev_texture);
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

    //if (m_prev_texture != -1) glDeleteTextures(1, &m_prev_texture); // OpenGL is very fragile
    //if (m_prev_texture == -1) m_prev_texture = m_texture;

    /*static int i = 0;
    i++;
    if (i == 10)
    {
        i = 0;
        
    }*/

    m_prev_texture = m_texture;

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Setup texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    bool is_grayscale = frame.channels() == 1;

    if (!is_grayscale)
    {
        // Upload the frame to the texture frame.data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frame.cols, frame.rows, 0, GL_RED, GL_UNSIGNED_BYTE, frame.data);
    }

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

std::shared_ptr<gl_cv_app::Shader> gl_cv_app::PostProcessingPipeline::getShaderIfAny()
{
    for (auto& effect : m_effects)
    {
        if (effect->isUsingCustomShader())
        {
            auto ptr = std::make_shared<gl_cv_app::Shader>(effect->getShaderPath());
            effect->setUniforms(ptr, this);
            return ptr;
        }
    }

    return nullptr;
}

std::pair<float, float> gl_cv_app::PostProcessingPipeline::getResolution()
{
    return {m_resolution.first, m_resolution.second};
}
