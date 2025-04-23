#include "Controller.h"

using namespace gl_utils;

namespace gl_cv_app
{
    uint NegativeEffect::m_id = 0;
    uint GrayscaleEffect::m_id = 1;
    uint BlurEffect::m_id = 2;
    uint EdgesEffect::m_id = 3;
    uint ContourEffect::m_id = 4;
    uint TriangulationEffect::m_id = 5;
    uint DenoisingEffect::m_id = 6;
    uint AcidEffect::m_id = 7;
    uint ChromaticAberrationEffect::m_id = 8;

    Controller::~Controller()
    {
    }

    bool Controller::isUsingCustomShader(std::shared_ptr<Shader>& sh)
    {
        auto ptr = model->getShaderIfAny();
        
        if (ptr == nullptr)
            return false;
        else
        {
            sh = std::move(ptr);
            return true;
        }
    }

    void Controller::bindEvents()
    {
        view->events.NegativeChanged += std::bind(&Controller::onBoolChanged<NegativeEffect>, this, std::placeholders::_1);
        view->events.GrayscaleChanged += std::bind(&Controller::onBoolChanged<GrayscaleEffect>, this, std::placeholders::_1);
        view->events.BlurChanged += std::bind(&Controller::onBlurChanged, this, std::placeholders::_1, std::placeholders::_2);
        view->events.EdgesChanged += std::bind(&Controller::onEdgesChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        view->events.ContoursChanged += std::bind(&Controller::onCountoursChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        view->events.TriangulationChanged += std::bind(&Controller::onTriangulationChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        view->events.DenoisingChanged += std::bind(&Controller::onDenoisingChanged, this, std::placeholders::_1, std::placeholders::_2);
        view->events.AcidChanged += std::bind(&Controller::onAcidChanged, this, std::placeholders::_1);
        view->events.AberrationChanged += std::bind(&Controller::onAberrationChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
    }

    bool Controller::checkIfConflicts(unsigned int id)
    {
        for (const auto& [key, value] : m_active_effects)
        {
            if (conflict_matrix[id][key])
                return true;
        }
        return false;
    }

    template <typename TEffect>
    void Controller::onBoolChanged(bool flag)
    {
        if (flag)
        {
            auto id = TEffect::getID();
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                switch (id)
                {
                case 0:
                    Renderer::UIState::isNegative = false;
                    break;
                case 1:
                    Renderer::UIState::isGrayscale = false;
                    break;
                default:
                    throw;
                }
                return;
            }
            auto effect_ptr = std::make_shared<TEffect>();
            model->addEffect(effect_ptr);
            m_active_effects.insert({ id, effect_ptr});
        }
        else
        {
            auto res = m_active_effects[TEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(TEffect::getID());
        }
    }

    void Controller::onBlurChanged(bool flag, float radius)
    {
        auto id = BlurEffect::getID();

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isBlur = false;
                return;
            }

            int kernel_size = 2 * static_cast<int>(radius) + 1;
            double sigma = static_cast<double>(radius) / 3.0;
            auto blur = std::make_shared<BlurEffect>(kernel_size, sigma);
            
            if (m_active_effects.insert({ id, blur}).second == true)
                model->addEffect(blur);
            else
            {
                auto& res = m_active_effects[id];
                model->removeEffect(res);
                m_active_effects.erase(id);
                m_active_effects.insert({ id, blur });
                model->addEffect(blur);
            }
        }
        else
        {
            auto& res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onEdgesChanged(bool flag, float lower_threshold, float upper_threshold)
    {
        auto id = EdgesEffect::getID();

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isEdges = false;
                return;
            }

            auto iter = m_active_effects.find(id);
            if (iter != m_active_effects.end())
            {
                dynamic_cast<EdgesEffect&>(*(iter->second)).setThresholds((double)lower_threshold, (double)upper_threshold);
            }
            else
            {
                auto edges = std::make_shared<EdgesEffect>((double)lower_threshold, (double)upper_threshold);
                model->addEffect(edges);
                m_active_effects.insert({ id, edges });
            }
        }
        else
        {
            auto& res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onCountoursChanged(bool flag, float threshold, ImVec4 color, int thickness)
    {
        auto id = ContourEffect::getID();
        cv::Scalar color_byte;
        cv::normalize(cv::Scalar(color.x, color.y, color.z, 1.0f), color_byte, 0, 255, cv::NORM_MINMAX);

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isContours = false;
                return;
            }

            auto iter = m_active_effects.find(id);
            if (iter != m_active_effects.end())
            {
                auto& ref = dynamic_cast<ContourEffect&>(*(iter->second));
                ref.setThreshold((double)threshold);
                ref.setColor(color_byte);
                ref.setThickness(thickness);
            }
            else
            {
                auto contours = std::make_shared<ContourEffect>((double)threshold, color_byte, thickness);
                model->addEffect(contours);
                m_active_effects.insert({ id, contours });
            }
        }
        else
        {
            auto& res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onTriangulationChanged(bool flag, bool delaunay, ImVec4 color, int threshold, bool is_drawing_centers)
    {
        auto id = TriangulationEffect::getID();
        cv::Scalar color_byte;
        cv::normalize(cv::Scalar(color.x, color.y, color.z, 1.0f), color_byte, 0, 255, cv::NORM_MINMAX);

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isTriangulation = false;
                return;
            }

            auto iter = m_active_effects.find(id);
            if (iter != m_active_effects.end())
            {
                auto& ref = dynamic_cast<TriangulationEffect&>(*(iter->second));
                ref.setDelaunay(delaunay);
                ref.setColor(color_byte);
                ref.setThreshold(threshold);
                ref.setDrawingCenters(is_drawing_centers);
            }
            else
            {
                auto triangulation = std::make_shared<TriangulationEffect>(delaunay, color_byte, threshold, is_drawing_centers);
                model->addEffect(triangulation);
                m_active_effects.insert({ id, triangulation });
            }
        }
        else
        {
            auto res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onDenoisingChanged(bool flag, float strength)
    {
        auto id = DenoisingEffect::getID();

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isDenoising = false;
                return;
            }

            auto iter = m_active_effects.find(id);
            if (iter != m_active_effects.end())
            {
                dynamic_cast<DenoisingEffect&>(*(iter->second)).setFilterStrength(strength);
            }
            else
            {
                auto denoising = std::make_shared<DenoisingEffect>(strength);
                model->addEffect(denoising);
                m_active_effects.insert({ id, denoising });
            }
        }
        else
        {
            auto res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onAcidChanged(bool flag)
    {
        auto id = AcidEffect::getID();
        model->setResolution(view->getWindowSize());

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isAcid = false;
                return;
            }

            auto effect_ptr = std::make_shared<AcidEffect>();
            model->addEffect(effect_ptr);
            m_active_effects.insert({ id, effect_ptr });
        }
        else
        {
            auto res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }

    void Controller::onAberrationChanged(bool flag, float redOffset, float greenOffset, float blueOffset)
    {
        auto id = ChromaticAberrationEffect::getID();

        if (flag)
        {
            if (checkIfConflicts(id))
            {
                view->setIsShowingConflict(true);
                Renderer::UIState::isAberration = false;
                return;
            }

            float rOffset = redOffset / 100.0f;
            float gOffset = greenOffset / 100.0f;
            float bOffset = blueOffset / 100.0f;

            auto iter = m_active_effects.find(id);
            if (iter != m_active_effects.end())
            {
                dynamic_cast<ChromaticAberrationEffect&>(*(iter->second)).setOffset(rOffset, gOffset, bOffset);
            }
            else
            {
                auto chrom = std::make_shared<ChromaticAberrationEffect>(rOffset, gOffset, bOffset);
                model->addEffect(chrom);
                m_active_effects.insert({ id, chrom });
            }
        }
        else
        {
            auto& res = m_active_effects[id];
            model->removeEffect(res);
            m_active_effects.erase(id);
        }
    }
    
}