#pragma once

#include "OISController.hpp"

namespace Core
{
    namespace Input
    {
        using namespace OIS;
        class OISKeyboard : public OISController<Keyboard, KeyListener>
        {
        public:

            OISKeyboard(InputManager* m_Input = NULL) : 
                OISController(m_Input, 256, OIS::Type::OISKeyboard)
            {
                for (size_t i = 0; i < m_Controls.size(); ++i)
                {
                    SetControl<bool>(static_cast<KeyCode>(i));
                }
            }

            virtual ~OISKeyboard()
            {

            }

        protected:

            virtual bool keyPressed(const KeyEvent& arg)
            {
                SetControlValue(arg.key, true);
                return true;
            }

            virtual bool keyReleased(const KeyEvent& arg)
            {
                SetControlValue(arg.key, false);
                return true;
            }

            template<typename T>
            void SetControl(KeyCode key)
            {
                Controller::SetControl<T>(static_cast<int>(key), Bindings::Convert(key));
            }
        };
    }
}
