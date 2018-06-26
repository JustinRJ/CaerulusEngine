#pragma once

#include "OISController.hpp"

namespace Core
{
    namespace Input
    {
        using namespace OIS;
        class OISMouse : public OISController<Mouse, MouseListener>
        {
        public:

            OISMouse(InputManager* m_Input = nullptr) : 
                OISController(m_Input, 12, OIS::Type::OISMouse)
            {
                for (int i = 0; i <= (int)MouseButton::MOUSE_BUTTON7; ++i)
                {
                    SetControl<bool>((MouseButton)i);
                }
                SetControl<glm::vec2>(MouseButton::MOUSE_XY);
                SetControl<glm::vec2>(MouseButton::MOUSE_XY_DELTA);
                SetControl<float>(MouseButton::MOUSE_SCROLL);
                SetControl<float>(MouseButton::MOUSE_SCROLL_DELTA);
            }

            virtual ~OISMouse()
            {
            }

            virtual void Update()
            {
                // Required as mouseMoved doesn't trigger every frame
                SetControlValue(static_cast<int>(MouseButton::MOUSE_XY_DELTA), glm::vec2(0.0f));
                SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL_DELTA), 0.0f);
                OISController::Update();
            }

        protected:

            virtual bool mouseMoved(const MouseEvent& arg)
            {
                const MouseState& ms = arg.state;
                SetControlValue(static_cast<int>(MouseButton::MOUSE_XY), glm::vec2((float)ms.X.abs, (float)ms.Y.abs));
                SetControlValue(static_cast<int>(MouseButton::MOUSE_XY_DELTA), glm::vec2((float)ms.X.rel, (float)ms.Y.rel));
                SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL), (float)ms.Z.abs);
                SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL_DELTA), (float)ms.Z.rel);
                return true;
            }

            virtual bool mousePressed(const MouseEvent& /*arg*/, MouseButtonID id)
            {
                SetControlValue(id, true);
                return true;
            }

            virtual bool mouseReleased(const MouseEvent& /*arg*/, MouseButtonID id)
            {
                SetControlValue(id, false);
                return true;
            }

            template<typename T>
            void SetControl(MouseButton key)
            {
                Controller::SetControl<T>(static_cast<int>(key), Bindings::Convert(key));
            }
        };
    }
}
