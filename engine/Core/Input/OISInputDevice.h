#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "OIS.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "InputDevice.hpp"

namespace Core
{
    namespace Input
    {
        class OISInputDevice : public InputDevice
        {
        public:

            CAERULUS_CORE OISInputDevice();
            CAERULUS_CORE virtual ~OISInputDevice();

            CAERULUS_CORE virtual void Initialize(void* hwnd) override;
            CAERULUS_CORE virtual void Uninitialize() override;

        protected:
            OIS::InputManager* m_Input;
        };
    }
}
