#pragma once

#define CAERULUS_INPUT __declspec(dllexport)

#include "OIS.h"
#include "InputDevice.hpp"

namespace Input
{
    class CAERULUS_INPUT OISInputDevice : public InputDevice
    {
    public:

        OISInputDevice();
        virtual ~OISInputDevice();

        virtual void Initialize(void* hwnd) override;
        virtual void Uninitialize() override;

    private:

        OIS::InputManager* m_Input;
    };
}
