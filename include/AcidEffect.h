#pragma once
#include "Effect.h"
#include "Shader.h"
#include "PostProcessingPipeline.h"

namespace gl_cv_app
{
    class AcidEffect : public Effect
    {
    public:
        AcidEffect(uint priority = 0) : Effect(priority) {};
        ~AcidEffect() {};
        static uint getID() { return m_id; };

        cv::Mat applyEffect(const cv::Mat& frame) override { return frame; }

        bool isUsingCustomShader() override { return true; };
        std::string getShaderPath() override { return "res\\shaders\\Acid.shader"; };
        void setUniforms(std::shared_ptr<gl_utils::Shader> shader, PostProcessingPipeline* pipe) override
        {
            auto time = pipe->getTime();
            float delta_time = time - m_prev_time;
            shader->bind();
            shader->setUniform1F("u_Time", time);
            //shader->SetUniform1f("u_DeltaTime", delta_time);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, pipe->getTexture());
            shader->setUniform1I("u_Texture", 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, pipe->getPrevTexture());
            shader->setUniform1I("u_PrevFrame", 1);
            
            auto resolution = pipe->getResolution();
            shader->setUniform2F("u_Resolution", resolution.first, resolution.second);

            glActiveTexture(GL_TEXTURE0);
            shader->unbind();
            m_prev_time = time;
        }

    private:
        static uint m_id;
        float m_prev_time = 0;
    };
}