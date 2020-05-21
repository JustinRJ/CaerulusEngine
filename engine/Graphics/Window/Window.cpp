#include "stdafx.h"

#include "Window.h"

namespace Graphics
{
    namespace Window
    {
        void Window::Set(const std::string& title, int x, int y, int bits, bool fullscreen)
        {
            m_newState = State(title, x, y, bits, fullscreen);
        }

        const State& Window::GetActiveState() const
        {
            return m_activeState;
        }

        bool Window::Compare(const State& state) const
        {
            return m_activeState.Compare(m_newState);
        }
    }
}
