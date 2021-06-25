#pragma once

#include <memory>
#include <glew.h>
#include <glfw3.h>

#include "IWindow.h"

namespace Core
{
    namespace Math
    {
        class Camera;
    }
}

namespace Graphics
{
    namespace Window
    {
        class CAERULUS_GRAPHICS GLWindow : public IWindow
        {
        public:
            GLWindow(std::shared_ptr<Core::Math::Camera> camera, const std::string& title, int x, int y, int bits, bool fullscreen = false);
            ~GLWindow();

            void Update() override;
            void Apply() override;
            void Focus() override;
            void SwapBuffer() const override;
            void* GetHandle() const override;

            GLFWwindow* GetGLFWWindow() const;
            
            void ToggleLockedCursor();
            bool IsCursorLocked() const;
            void CenterCursor();

        private:
            GLFWwindow* m_window;
            bool m_lockedCursor;
            std::shared_ptr<Core::Math::Camera> m_camera;
        };
    }
}
