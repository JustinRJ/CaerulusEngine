#pragma once

#define CAERULUS_CORE __declspec(dllexport)

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

            virtual void Initialize(/*Graphics::Window::IWindow* window*/)
            {
                //void* handle = window->Handle();
                //m_Input = InputManager::createInputSystem((size_t)handle);

                m_Controllers[Controllers::KEYBOARD] = new Input::OISKeyboard(m_Input);
                m_Controllers[Controllers::MOUSE] = new Input::OISMouse(m_Input);

                InputDevice::Initialize(/*window*/);
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
