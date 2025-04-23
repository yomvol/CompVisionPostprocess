#pragma once
#include "Effect.h"
#include "Shader.h"
#include "PostProcessingPipeline.h"

namespace gl_cv_app
{
    class ChromaticAberrationEffect : public Effect
    {
    public:
        ChromaticAberrationEffect(float redOffset, float greenOffset, float blueOffset, uint priority = 0) : Effect(priority) {
            m_offset[0] = redOffset;
            m_offset[1] = greenOffset;
            m_offset[2] = blueOffset;
        };
        ~ChromaticAberrationEffect() {};
        static uint getID() { return m_id; };

        cv::Mat applyEffect(const cv::Mat& frame) override { return frame; }

        bool isUsingCustomShader() override { return true; };
        std::string getShaderPath() override { return "res\\shaders\\Aberration.shader"; };

        void setUniforms(std::shared_ptr<gl_utils::Shader> shader, PostProcessingPipeline* pipe) override
        {
            shader->bind();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pipe->getTexture());
            shader->setUniform1I("u_Texture", 0);
            shader->setUniform3F("u_Offset", m_offset[0], m_offset[1], m_offset[2]);
            shader->unbind();
        }
        void setOffset(float redOffset, float greenOffset, float blueOffset)
        {
            m_offset[0] = redOffset;
            m_offset[1] = greenOffset;
            m_offset[2] = blueOffset;
        }

    private:
        static uint m_id;
        float m_offset[3];
    };
}