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
            GLWindow(Core::Math::Camera* camera, const std::string& title, int x, int y, int bits, bool fullscreen = false);
            ~GLWindow();

            void Update() override;
            void Apply() override;
            void Focus() override;
            void SwapBuffer() override;
            void* GetHandle() const override;

            GLFWwindow* GetGLFWWindow();
            
            void ToggleLockedCursor();
            bool IsCursorLocked() const;
            void CenterCursor();

        private:
            bool m_lockedCursor;
            GLFWwindow* m_window;
            Core::Math::Camera* m_camera;
        };
    }
}
