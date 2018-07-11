#include "stdafx.h"

#include "OISMouse.h"
#include "../Math/MathHelper.h"

namespace Core
{
    namespace Input
    {
        using namespace OIS;

        OISMouse::OISMouse(InputManager* m_Input) :
            OISController(m_Input, 12, OIS::Type::OISMouse)
        {
            for (int i = 0; i <= static_cast<int>(MouseButton::MOUSE_BUTTON7); ++i)
            {
                SetControl<bool>((MouseButton)i);
            }
            SetControl<glm::vec2>(MouseButton::MOUSE_XY);
            SetControl<glm::vec2>(MouseButton::MOUSE_XY_DELTA);
            SetControl<float>(MouseButton::MOUSE_SCROLL);
            SetControl<float>(MouseButton::MOUSE_SCROLL_DELTA);
        }

        OISMouse::~OISMouse()
        {
        }

        void OISMouse::Update()
        {
            // Required as mouseMoved doesn't trigger every frame
            SetControlValue(static_cast<int>(MouseButton::MOUSE_XY_DELTA), glm::vec2(0.0f));
            SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL_DELTA), 0.0f);
            OISController::Update();
        }

        bool OISMouse::mouseMoved(const MouseEvent& arg)
        {
            const MouseState& ms = arg.state;
            SetControlValue(static_cast<int>(MouseButton::MOUSE_XY),        glm::vec2(static_cast<float>(ms.X.abs), static_cast<float>(ms.Y.abs)));
            SetControlValue(static_cast<int>(MouseButton::MOUSE_XY_DELTA),  glm::vec2(static_cast<float>(ms.X.rel), static_cast<float>(ms.Y.rel)));
            SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL),        static_cast<float>(ms.Z.abs));
            SetControlValue(static_cast<int>(MouseButton::MOUSE_SCROLL_DELTA),  static_cast<float>(ms.Z.rel));
            return true;
        }

        bool OISMouse::mousePressed(const MouseEvent& arg, MouseButtonID id)
        {
            SetControlValue(id, true);
            return true;
        }

        bool OISMouse::mouseReleased(const MouseEvent& arg, MouseButtonID id)
        {
            SetControlValue(id, false);
            return true;
        }
    }
}
