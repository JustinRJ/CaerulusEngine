#include "stdafx.h"

#include "GLWindow.h"
#include <glew.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

namespace Graphics
{
    namespace Window
    {
        GLWindow::GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen) :
            m_Window(nullptr),
            m_LockedCursor(false)
        {
            Window::Set(title, x, y, bits, fullscreen);
            Apply();
        }

        GLWindow::~GLWindow()
        {
            glfwDestroyWindow(m_Window);
        }

        void* GLWindow::GetHandle() const
        {
            return glfwGetWin32Window(m_Window);
        }

        void GLWindow::Apply()
        {
            if (!Window::Compare(m_NewState))
            {
                m_ActiveState = m_NewState;
                m_Window = glfwCreateWindow(m_ActiveState.Width, m_ActiveState.Height, m_ActiveState.Title.c_str(), nullptr, nullptr);
                glfwMakeContextCurrent(m_Window);
            }
        }

        void GLWindow::Focus()
        {
            glfwFocusWindow(m_Window);
        }

        void GLWindow::SwapBuffer() const
        {
            glfwSwapBuffers(m_Window);
        }

        GLFWwindow* GLWindow::GetGLFWWindow() const
        {
            return m_Window;
        }

        void GLWindow::CenterCursor() const
        {
            glfwSetCursorPos(m_Window, m_ActiveState.Width / 2, m_ActiveState.Height / 2);
        }

        void GLWindow::ToggleLockedCursor()
        {
            if (m_LockedCursor)
            {
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else
            {
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                CenterCursor();
            }
            m_LockedCursor = !m_LockedCursor;
        }
    }
}
