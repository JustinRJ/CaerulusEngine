#include "stdafx.h"

#include "OISInputDevice.h"
#include "OISMouse.h"
#include "OISKeyboard.h"

namespace Core
{
    namespace Input
    {
        using namespace OIS;

        OISInputDevice::OISInputDevice() :
            m_Input(nullptr)
        {
        }

        OISInputDevice::~OISInputDevice()
        {
            Uninitialize();
        }

        void OISInputDevice::Initialize(void* hwnd)
        {
            m_Input = InputManager::createInputSystem((size_t)hwnd);
            m_Controllers[Controllers::KEYBOARD] = new Input::OISKeyboard(m_Input);
            m_Controllers[Controllers::MOUSE] = new Input::OISMouse(m_Input);
            InputDevice::Initialize(hwnd);
        }

        void OISInputDevice::Uninitialize()
        {
            InputDevice::Uninitialize();
            if (m_Input)
            {
                InputManager::destroyInputSystem(m_Input);
            }
        }

    }
}
