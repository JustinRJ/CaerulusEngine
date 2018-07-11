#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "OIS.h"
#include "OISController.hpp"

namespace Core
{
    namespace Input
    {
        class OISMouse : public OISController<OIS::Mouse, OIS::MouseListener>
        {
        public:

            CAERULUS_CORE OISMouse(OIS::InputManager* m_Input = nullptr);
            CAERULUS_CORE virtual ~OISMouse();

            CAERULUS_CORE virtual void Update() override;

        protected:

            virtual bool mouseMoved(const OIS::MouseEvent& arg) override;
            virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;
            virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id) override;

            template<typename T>
            void SetControl(MouseButton key)
            {
                Controller::SetControl<T>(static_cast<int>(key), Bindings::Convert(key));
            }
        };
    }
}
