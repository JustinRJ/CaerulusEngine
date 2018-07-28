#pragma once

#include "Control.hpp"

#include <vector>

namespace Core
{
    namespace Input
    {
        class Controller
        {
        public:

            Controller(int controls) :
                m_Controls(controls)
            {
            }

            virtual ~Controller()
            {
                m_Controls.clear();
            }

            virtual void Update() = 0;

            virtual std::shared_ptr<Control> GetControl(int key) const
            {
                return m_Controls.at(key);
            }

            template<typename T>
            T GetControlValue(int key) const
            {
                return GetControl<T>(key)->Value;
            }

            template<typename T>
            void SetControlValue(int key, T value)
            {
                GetControl<T>(key)->Value = value;
            }

        protected:

            std::vector<std::shared_ptr<Control>> m_Controls;

            template<typename T>
            void SetControl(int key, const std::string& name)
            {
                m_Controls[key] = std::make_shared<ControlT<T>>(name);
            }

            template<typename T>
            std::shared_ptr<ControlT<T>> GetControl(int key) const
            {
                return std::dynamic_pointer_cast<ControlT<T>>(m_Controls.at(key));
            }
        };
    }
}