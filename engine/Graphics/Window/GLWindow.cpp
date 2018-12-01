#include "stdafx.h"

#include "GLWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

namespace Graphics
{
    namespace Window
    {
        GLWindow::GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen) :
            m_Window(nullptr)
        {
            //Move to graphics system constructor
            glfwInit();

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

        GLFWwindow* GLWindow::GetWindow() const
        {
            return m_Window;
        }
    }
}
