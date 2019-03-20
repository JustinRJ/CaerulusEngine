#pragma once

#define CAERULUS_INPUT __declspec(dllexport)

#include "OIS.h"
#include "InputDevice.hpp"

namespace Input
{
    class OISInputDevice : public InputDevice
    {
    public:

        CAERULUS_INPUT OISInputDevice();
        CAERULUS_INPUT virtual ~OISInputDevice();

        CAERULUS_INPUT virtual void Initialize(void* hwnd) override;
        CAERULUS_INPUT virtual void Uninitialize() override;

    private:

        OIS::InputManager* m_Input;
    };
}
