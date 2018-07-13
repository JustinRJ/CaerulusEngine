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
                for (size_t i = 0; i < m_Controls.size(); ++i)
                {
                    delete m_Controls[i];
                }
                m_Controls.clear();
            }

            virtual void Update() = 0;

            virtual Control* GetControl(int key)
            {
                return m_Controls[key];
            }

            template<typename T>
            T GetControlValue(int key)
            {
                return GetControl<T>(key)->Value;
            }

            template<typename T>
            void SetControlValue(int key, T value)
            {
                GetControl<T>(key)->Value = value;
            }

        protected:

            std::vector<Control*> m_Controls;

            template<typename T>
            void SetControl(int key, const std::string& name)
            {
                m_Controls[key] = new ControlT<T>(name);
            }

            template<typename T>
            ControlT<T>* GetControl(int key)
            {
                return static_cast<ControlT<T>*>(m_Controls[key]);
            }
        };
    }
}