#include "stdafx.h"

#include "GLWindow.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#include <iostream>
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace Window
    {
        GLWindow::GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen) :
            m_Window(nullptr),
            m_LockedCursor(true)
        {
            if (!glfwInit())
            {
                std::cerr << "Failed to init GLFW!" << std::endl;
                exit(1);
            }

            Window::Set(title, x, y, bits, fullscreen);
            Apply();
            CenterCursor();
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
                m_Window = glfwCreateWindow(
                    m_ActiveState.Width,
                    m_ActiveState.Height,
                    m_ActiveState.Title.c_str(),
                    m_ActiveState.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                    nullptr);
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

        void GLWindow::Update()
        {
            Apply();
            if (m_LockedCursor)
            {
                CenterCursor();
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            else
            {
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }

        void GLWindow::CenterCursor()
        {
            glfwSetCursorPos(m_Window, m_ActiveState.Width / 2, m_ActiveState.Height / 2);
        }

        void GLWindow::ToggleLockedCursor()
        {
            CenterCursor();
            m_LockedCursor = !m_LockedCursor;
        }

        bool GLWindow::IsCursorLocked() const
        {
            return m_LockedCursor;
        }

        Geometry::QuadGeometry* GLWindow::GetQuad() const
        {
            return m_Quad;
        }

        void GLWindow::SetQuad(Geometry::QuadGeometry* quad)
        {
            m_Quad = quad;
        }
    }
}
