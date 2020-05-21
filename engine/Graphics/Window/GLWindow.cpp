#include "stdafx.h"

#include "GLWindow.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#include "../../Core/Math/Math.h"
#include "../../Core/Logging/Log.h"

namespace Graphics
{
    namespace Window
    {
        GLWindow::GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen) :
            m_window(nullptr),
            m_lockedCursor(true)
        {
            if (!glfwInit())
            {
                using Core::Logging::Log;
                Log::LogError("Failed to init GLFW!");
                exit(1);
            }

            Window::Set(title, x, y, bits, fullscreen);
            Apply();
            CenterCursor();
        }

        void* GLWindow::GetHandle() const
        {
            return glfwGetWin32Window(m_window);
        }

        void GLWindow::Apply()
        {
            if (!Window::Compare(m_newState))
            {
                m_activeState = m_newState;
                m_window = glfwCreateWindow(
                    m_activeState.Width,
                    m_activeState.Height,
                    m_activeState.Title.c_str(),
                    m_activeState.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                    nullptr);
                glfwMakeContextCurrent(m_window);
            }
        }

        void GLWindow::Focus()
        {
            glfwFocusWindow(m_window);
        }

        void GLWindow::SwapBuffer() const
        {
            glfwSwapBuffers(m_window);
        }

        GLFWwindow* GLWindow::GetGLFWWindow() const
        {
            return m_window;
        }

        void GLWindow::Update()
        {
            Apply();
            if (m_lockedCursor)
            {
                CenterCursor();
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else
            {
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        void GLWindow::CenterCursor()
        {
            glfwSetCursorPos(m_window, m_activeState.Width / 2, m_activeState.Height / 2);
        }

        void GLWindow::ToggleLockedCursor()
        {
            CenterCursor();
            m_lockedCursor = !m_lockedCursor;
        }

        bool GLWindow::IsCursorLocked() const
        {
            return m_lockedCursor;
        }

        std::shared_ptr<Quad> GLWindow::GetQuad() const
        {
            return m_quad;
        }

        void GLWindow::SetQuad(std::shared_ptr<Quad> quad)
        {
            m_quad = quad;
        }
    }
}
