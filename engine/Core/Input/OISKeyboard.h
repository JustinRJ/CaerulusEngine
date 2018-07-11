#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "OIS.h"
#include "OISController.hpp"

namespace Core
{
    namespace Input
    {
        class OISKeyboard : public OISController<OIS::Keyboard, OIS::KeyListener>
        {
        public:

            CAERULUS_CORE OISKeyboard(OIS::InputManager* m_Input = nullptr);
            CAERULUS_CORE virtual ~OISKeyboard();

        protected:

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
