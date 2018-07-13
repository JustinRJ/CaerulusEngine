#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "OIS.h"
#include "InputDevice.hpp"

namespace Core
{
    namespace Input
    {
        class OISInputDevice : public InputDevice
        {
        public:

            CAERULUS_CORE OISInputDevice();
            virtual ~OISInputDevice();

            virtual void Initialize(void* hwnd) override;
            virtual void Uninitialize() override;

        private:

            OIS::InputManager* m_Input;
        };
    }
}
