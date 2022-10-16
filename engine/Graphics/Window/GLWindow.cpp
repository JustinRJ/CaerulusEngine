#include "stdafx.h"

#include "Window/GLWindow.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>

#include "Math/Math.h"
#include "Math/Camera.h"
#include "Log/Log.h"

using namespace Core::Log;

void CloseWindowCallback(GLFWwindow* window)
{
    exit(1);
}

namespace Graphics
{
    namespace Window
    {
        GLWindow::GLWindow(Core::Math::Camera* camera, std::string_view title, int x, int y, int bits, bool fullscreen) :
            m_lockedCursor(true),
            m_window(nullptr),
            m_camera(camera)
        {
            if (!glfwInit())
            {
                LogError("Failed to init GLFW!");
                exit(1);
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_SAMPLES, 4);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            IWindow::Set(title, x, y, bits, fullscreen);
            Apply();
            CenterCursor();
            glfwSetWindowCloseCallback(m_window, CloseWindowCallback);
        }

        GLWindow::~GLWindow()
        {
            glfwTerminate();
        }

        void* GLWindow::GetHandle() const
        {
            return glfwGetWin32Window(m_window);
        }

        void GLWindow::Apply()
        {
            if (!IWindow::Compare(m_newState))
            {
                m_activeState = m_newState;
                m_window = glfwCreateWindow(
                    m_activeState.Width,
                    m_activeState.Height,
                    m_activeState.Title.c_str(),
                    m_activeState.Fullscreen ? glfwGetPrimaryMonitor() : nullptr,
                    nullptr);
                glfwMakeContextCurrent(m_window);

                int width, height;
                glfwGetFramebufferSize(m_window, &width, &height);
                glViewport(0, 0, width, height);
            }
        }

        void GLWindow::Focus()
        {
            glfwFocusWindow(m_window);
        }

        void GLWindow::SwapBuffer()
        {
            glfwSwapBuffers(m_window);
        }

        GLFWwindow* GLWindow::GetGLFWWindow()
        {
            return m_window;
        }

        void GLWindow::Update()
        {
            if (m_camera)
            {
                m_camera->GetPerspective().SetAspect(static_cast<float>(GetActiveState().Width) / static_cast<float>(GetActiveState().Height));
            }

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
            glfwSetCursorPos(m_window, m_activeState.Width * 0.5, m_activeState.Height * 0.5);
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
    }
}
