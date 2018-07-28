#pragma once

#include "OIS.h"
#include "OISController.hpp"
#include "Bindings.h"

namespace Core
{
    namespace Input
    {
        class OISKeyboard : public OISController<OIS::Keyboard, OIS::KeyListener>
        {
        public:

            OISKeyboard(OIS::InputManager* m_Input = nullptr);
            virtual ~OISKeyboard();

        private:

            virtual bool keyPressed(const OIS::KeyEvent& arg) override;
            virtual bool keyReleased(const OIS::KeyEvent& arg) override;

            template<typename T>
            void SetControl(OIS::KeyCode key)
            {
                Controller::SetControl<T>(static_cast<int>(key), Bindings::Convert(key));
            }
        };
    }
}
