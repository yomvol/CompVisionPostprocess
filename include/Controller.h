#pragma once
#include "PostProcessingPipeline.h"
#include "Renderer.h"
#include "NegativeEffect.h"
#include "EdgesEffect.h"
#include "ContourEffect.h"
#include "TriangulationEffect.h"
#include "DenoisingEffect.h"
#include "AcidEffect.h"

namespace gl_cv_app
{
    class Renderer;

    /// <summary>
    /// Controller component mediates communication between the view and the model. Here are stored event handlers.
    /// </summary>
    class Controller
    {
    public:
        explicit Controller(Renderer* v, PostProcessingPipeline* m) : view(v), model(m) { bindEvents(); v->controller = this; };
        ~Controller();
        bool isUsingCustomShader(std::shared_ptr<Shader>& sh); // view asks model if we need custom shader. Just in case passes pointer.

        Renderer* view;
        PostProcessingPipeline* model;

    private:
        std::unordered_map<unsigned int, std::shared_ptr<Effect>> m_active_effects;

        void bindEvents();

        template <typename TEffect>
        void onBoolChanged(bool flag);

        void onBlurChanged(bool flag, float radius);
        void onEdgesChanged(bool flag, float lower_threshold, float upper_threshold);
        void onCountoursChanged(bool flag, float threshold, ImVec4 color, int thickness);
        void onTriangulationChanged(bool flag, bool delaunay, ImVec4 color, int threshold, bool is_drawing_centers);
        void onDenoisingChanged(bool flag, float strength);
        void onAcidChanged(bool flag);

        //void OnNegativeChanged(bool flag);
        //void OnGrayscaleChanged(bool flag);
    };
}