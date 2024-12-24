#pragma once
#include "PostProcessingPipeline.h"
#include "Renderer.h"
#include "NegativeEffect.h"

namespace gl_cv_app
{
    /// <summary>
    /// Controller component mediates communication between the view and the model. Here are stored event handlers.
    /// </summary>
    class Controller
    {
    public:
        explicit Controller(Renderer* v, PostProcessingPipeline* m) : view(v), model(m) { bindEvents(); };
        ~Controller();
        Renderer* view;
        PostProcessingPipeline* model;

    private:
        std::vector<std::shared_ptr<Effect>> m_active_effects;

        void bindEvents();
        void OnButtonClick();
        void OnNegativeChanged(bool flag);
    };
}