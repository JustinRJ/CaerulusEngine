#include "stdafx.h"
#include "Window.h"

namespace Graphics
{
    namespace Window
    {
        Window::Window() 
        {
        }

        void Window::Set(const std::string& title, int x, int y, int bits, bool fullscreen)
        {
            m_NewState = State(title, x, y, bits, fullscreen);
        }

        Window::~Window()
        {
        }

        const State& Window::GetActiveState() const
        {
            return m_ActiveState;
        }

        bool Window::Compare(const State& state) const
        {
            return m_ActiveState.Compare(m_NewState);
        }
    }
}
