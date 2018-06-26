#pragma once

#include "Bindings.h"

namespace Core
{
    namespace Input
    {
        template <typename T, typename L>
        class OISController : public Controller, public L
        {
        public:
            OISController(OIS::InputManager* m_Input, int controls, OIS::Type device, bool buffered = true) : 
                Controller(controls),
                m_Input(m_Input),
                m_Device(nullptr)
            {
                CreateDevice(device, buffered);
            }

            virtual ~OISController()
            {
                if (m_Input && m_Device)
                {
                    m_Input->destroyInputObject(m_Device);
                }
            }

            virtual void Update()
            {
                m_Device->capture();
            }

            protected:

                OIS::InputManager* m_Input;
                T* m_Device;

                virtual void CreateDevice(OIS::Type key, bool buffered)
                {
                    m_Device = static_cast<T*>(m_Input->createInputObject(key, buffered));
                    m_Device->setEventCallback(this);
                }
        };
    }
}
