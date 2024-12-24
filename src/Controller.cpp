#include "Controller.h"

namespace gl_cv_app
{
    Controller::~Controller()
    {
    }

    void Controller::bindEvents()
    {
        view->registerEvent("OnNegativeChanged", static_cast<std::function<void()>>(std::bind(&Controller::OnButtonClick, this)));
        view->registerEvent("OnNegativeChanged", std::bind(&Controller::OnNegativeChanged, this, std::placeholders::_1));
    }

    void Controller::OnButtonClick()
    {
        std::cout << "\nTestEvent\n";
    }

    void Controller::OnNegativeChanged(bool flag)
    {
        std::cout << "\nNegativeChanged: " << flag << std::endl;
        if (flag)
        {
            auto neg = std::make_shared<NegativeEffect>(0, 0);
            model->addEffect(neg);
            m_active_effects.push_back(neg);
        }
        else
        {
            auto res = std::find_if(m_active_effects.begin(), m_active_effects.end(), [](const std::shared_ptr<Effect>& effect) {
                return effect->getID() == 0;
                });
            if (res != m_active_effects.end())
            {
                model->removeEffect(*res);
                m_active_effects.erase(res);
            }
        }
    }
    
}