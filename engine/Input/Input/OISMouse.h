#pragma once

#include "OIS.h"
#include "OISController.hpp"
#include "Bindings.h"
#include "InputEnums.h"

namespace Input
{
    class OISMouse : public OISController<OIS::Mouse, OIS::MouseListener>
    {
    public:

        OISMouse(OIS::InputManager* m_Input = nullptr);
        virtual ~OISMouse();

        virtual void Update() override;

    private:

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
