#include "Controller.h"

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
        //view->events.NegativeChanged += std::bind(&Controller::OnNegativeChanged, this, std::placeholders::_1);
        view->events.NegativeChanged += std::bind(&Controller::onBoolChanged<NegativeEffect>, this, std::placeholders::_1);
        view->events.GrayscaleChanged += std::bind(&Controller::onBoolChanged<GrayscaleEffect>, this, std::placeholders::_1);
        view->events.BlurChanged += std::bind(&Controller::onBlurChanged, this, std::placeholders::_1, std::placeholders::_2);
        view->events.EdgesChanged += std::bind(&Controller::onEdgesChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        view->events.ContoursChanged += std::bind(&Controller::onCountoursChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        view->events.TriangulationChanged += std::bind(&Controller::onTriangulationChanged, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        view->events.DenoisingChanged += std::bind(&Controller::onDenoisingChanged, this, std::placeholders::_1, std::placeholders::_2);
        view->events.AcidChanged += std::bind(&Controller::onAcidChanged, this, std::placeholders::_1);
    
    }

    template <typename TEffect>
    void Controller::onBoolChanged(bool flag)
    {
        if (flag)
        {
            auto effect_ptr = std::make_shared<TEffect>();
            model->addEffect(effect_ptr);
            m_active_effects.insert({ TEffect::getID(), effect_ptr});
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
        if (flag)
        {
            int kernel_size = 2 * static_cast<int>(radius) + 1;
            double sigma = static_cast<double>(radius) / 3.0;
            auto blur = std::make_shared<BlurEffect>(kernel_size, sigma);
            
            if (m_active_effects.insert({ BlurEffect::getID(), blur}).second == true)
                model->addEffect(blur);
            else
            {
                auto res = m_active_effects[BlurEffect::getID()];
                model->removeEffect(res);
                m_active_effects.erase(BlurEffect::getID());
                m_active_effects.insert({ BlurEffect::getID(), blur });
                model->addEffect(blur);
            }
        }
        else
        {
            auto res = m_active_effects[BlurEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(BlurEffect::getID());
        }
    }

    void Controller::onEdgesChanged(bool flag, float lower_threshold, float upper_threshold)
    {
        if (flag)
        {
            auto iter = m_active_effects.find(EdgesEffect::getID());
            if (iter != m_active_effects.end())
            {
                dynamic_cast<EdgesEffect&>(*(iter->second)).setThresholds((double)lower_threshold, (double)upper_threshold);
            }
            else
            {
                auto edges = std::make_shared<EdgesEffect>((double)lower_threshold, (double)upper_threshold);
                model->addEffect(edges);
                m_active_effects.insert({ EdgesEffect::getID(), edges });
            }
        }
        else
        {
            auto res = m_active_effects[EdgesEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(EdgesEffect::getID());
        }
    }

    void Controller::onCountoursChanged(bool flag, float threshold, ImVec4 color, int thickness)
    {
        cv::Scalar color_byte;
        cv::normalize(cv::Scalar(color.x, color.y, color.z, 1.0f), color_byte, 0, 255, cv::NORM_MINMAX);

        if (flag)
        {
            auto iter = m_active_effects.find(ContourEffect::getID());
            if (iter != m_active_effects.end())
            {
                dynamic_cast<ContourEffect&>(*(iter->second)).setThreshold((double)threshold);
                dynamic_cast<ContourEffect&>(*(iter->second)).setColor(color_byte);
                dynamic_cast<ContourEffect&>(*(iter->second)).setThickness(thickness);
            }
            else
            {
                auto contours = std::make_shared<ContourEffect>((double)threshold, color_byte, thickness);
                model->addEffect(contours);
                m_active_effects.insert({ ContourEffect::getID(), contours });
            }
        }
        else
        {
            auto res = m_active_effects[ContourEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(ContourEffect::getID());
        }
    }

    void Controller::onTriangulationChanged(bool flag, bool delaunay, ImVec4 color, int threshold, bool is_drawing_centers)
    {
        cv::Scalar color_byte;
        cv::normalize(cv::Scalar(color.x, color.y, color.z, 1.0f), color_byte, 0, 255, cv::NORM_MINMAX);

        if (flag)
        {
            auto iter = m_active_effects.find(TriangulationEffect::getID());
            if (iter != m_active_effects.end())
            {
                dynamic_cast<TriangulationEffect&>(*(iter->second)).setDelaunay(delaunay);
                dynamic_cast<TriangulationEffect&>(*(iter->second)).setColor(color_byte);
                dynamic_cast<TriangulationEffect&>(*(iter->second)).setThreshold(threshold);
                dynamic_cast<TriangulationEffect&>(*(iter->second)).setDrawingCenters(is_drawing_centers);
            }
            else
            {
                auto triangulation = std::make_shared<TriangulationEffect>(delaunay, color_byte, threshold, is_drawing_centers);
                model->addEffect(triangulation);
                m_active_effects.insert({ TriangulationEffect::getID(), triangulation });
            }
        }
        else
        {
            auto res = m_active_effects[TriangulationEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(TriangulationEffect::getID());
        }
    }

    void Controller::onDenoisingChanged(bool flag, float strength)
    {
        if (flag)
        {
            auto iter = m_active_effects.find(DenoisingEffect::getID());
            if (iter != m_active_effects.end())
            {
                dynamic_cast<DenoisingEffect&>(*(iter->second)).setFilterStrength(strength);
            }
            else
            {
                auto denoising = std::make_shared<DenoisingEffect>(strength);
                model->addEffect(denoising);
                m_active_effects.insert({ DenoisingEffect::getID(), denoising });
            }
        }
        else
        {
            auto res = m_active_effects[DenoisingEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(DenoisingEffect::getID());
        }
    }

    void Controller::onAcidChanged(bool flag)
    {
        model->setResolution(view->getWindowSize());

        if (flag)
        {
            auto effect_ptr = std::make_shared<AcidEffect>();
            model->addEffect(effect_ptr);
            m_active_effects.insert({ AcidEffect::getID(), effect_ptr });
        }
        else
        {
            auto res = m_active_effects[AcidEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(AcidEffect::getID());
        }
    }

    /*void Controller::OnNegativeChanged(bool flag)
    {
        if (flag)
        {
            auto neg = std::make_shared<NegativeEffect>(m_ID_counter);
            model->addEffect(neg);
            m_active_effects.insert({m_ID_counter, neg});
            m_ID_counter++;
        }
        else
        {
            auto res = m_active_effects[NegativeEffect::getID()];
            model->removeEffect(res);
            m_active_effects.erase(NegativeEffect::getID());
        }
    }*/

    /*void Controller::OnGrayscaleChanged(bool flag)
    {
        if (flag)
        {
            auto gray = std::make_shared<GrayscaleEffect>(1);
            model->addEffect(gray);
            m_active_effects.push_back(gray);
        }
        else
        {
            auto res = std::find_if(m_active_effects.begin(), m_active_effects.end(), [](const std::shared_ptr<Effect>& effect) {
                return effect->getID() == 1;
                });
            if (res != m_active_effects.end())
            {
                model->removeEffect(*res);
                m_active_effects.erase(res);
            }
        }
    }*/
    
}