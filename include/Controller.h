#pragma once
#include "PostProcessingPipeline.h"
#include "Renderer.h"
#include "NegativeEffect.h"
#include "EdgesEffect.h"
#include "ContourEffect.h"
#include "TriangulationEffect.h"
#include "DenoisingEffect.h"
#include "AcidEffect.h"
#include "ChromaticAberrationEffect.h"
#include <array>

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
        bool isUsingCustomShader(std::shared_ptr<gl_utils::Shader>& sh); // view asks model if we need custom shader. Just in case passes pointer.

        Renderer* view;
        PostProcessingPipeline* model;

    private:
        std::unordered_map<unsigned int, std::shared_ptr<Effect>> m_active_effects;

        void bindEvents();
        bool checkIfConflicts(unsigned int id);

        template <typename TEffect>
        void onBoolChanged(bool flag);

        void onBlurChanged(bool flag, float radius);
        void onEdgesChanged(bool flag, float lower_threshold, float upper_threshold);
        void onCountoursChanged(bool flag, float threshold, ImVec4 color, int thickness);
        void onTriangulationChanged(bool flag, bool delaunay, ImVec4 color, int threshold, bool is_drawing_centers);
        void onDenoisingChanged(bool flag, float strength);
        void onAcidChanged(bool flag);
        void onAberrationChanged(bool flag, float redOffset, float greenOffset, float blueOffset);

        enum class EffectType
        {
            Negative,
            Grayscale,
            Blur,
            Edges,
            Contours,
            Triangulation,
            Denoising,
            Acid,
            ChromaticAberration,
            Count // ensure that Count is the last here
        };

        constexpr static std::array<std::array<bool, (size_t)EffectType::Count>, (size_t)EffectType::Count> conflict_matrix = { {
            {false, true, false, true, true, true, false, true, true}, // Negative conflicts with all, except Blur amd Denoising
            {true, false, false, true, true, true, false, true, true}, // Grayscale conflicts with all, except Blur amd Denoising
            {false, false, false, true, false, true, true, true, true}, // Blur conflicts with Edges, Contours, Triangulation, Denoising and Acid
            {true, true, true, false, true, true, true, true, true}, // Edges conflicts with Negative, Grayscale, Contours, Triangulation, Denoising and Acid
            {true, true, false, true, false, true, false, true, true}, // Contours conflicts with Negative, Grayscale, Edges, Triangulation and Acid
            {true, true, true, true, true, false, true, true, true}, // Triangulation conflicts with Negative, Grayscale, Edges, Contours, Denoising and Acid
            {false, false, true, true, false, true, false, true, true}, // Denoising conflicts with Negative, Grayscale, Edges, Triangulation and Acid
            {true, true, true, true, true, true, true, false, true}, // Acid conflicts with Negative, Grayscale, Edges, Contours, Triangulation and Denoising
            {true, true, true, true, true, true, true, true, false} // Chromatic Aberration conflicts with all, except itself
            }};
    };
}