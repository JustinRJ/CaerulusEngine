#include "stdafx.h"

#include "OISKeyboard.h"

namespace Input
{
    using namespace OIS;

    OISKeyboard::OISKeyboard(InputManager* m_Input) :
        OISController(m_Input, 256, OIS::Type::OISKeyboard)
    {
        for (size_t i = 0; i < m_Controls.size(); ++i)
        {
            SetControl<bool>(static_cast<KeyCode>(i));
        }
    }

    OISKeyboard::~OISKeyboard()
    {
    }

    bool OISKeyboard::keyPressed(const KeyEvent& arg)
    {
        SetControlValue(arg.key, true);
        return true;
    }

    bool OISKeyboard::keyReleased(const KeyEvent& arg)
    {
        SetControlValue(arg.key, false);
        return true;
    }
}
