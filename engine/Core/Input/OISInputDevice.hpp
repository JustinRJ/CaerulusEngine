#pragma once

#include "OIS.h"
#include "InputDevice.hpp"
#include "OISMouse.hpp"
#include "OISKeyboard.hpp"

namespace Core
{
    namespace Input
    {
        class OISInputDevice : public InputDevice
        {
        protected:
            InputManager* m_Input;

        public:

            OISInputDevice() :
                m_Input(nullptr)
            {
            }

            virtual ~OISInputDevice()
            {
                Uninitialize();
            }

            virtual void Initialize(void* hwnd)
            {
                m_Input = InputManager::createInputSystem((size_t)hwnd);
                m_Controllers[Controllers::KEYBOARD] = new Input::OISKeyboard(m_Input);
                m_Controllers[Controllers::MOUSE] = new Input::OISMouse(m_Input);
                InputDevice::Initialize(hwnd);
            }

            virtual void Uninitialize()
            {
                InputDevice::Uninitialize();
                if (m_Input)
                {
                    InputManager::destroyInputSystem(m_Input);
                }
            }
        };
    }
}
