#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Window.h"

namespace Graphics
{
    namespace Window
    {
        class CAERULUS_GRAPHICS GLWindow : public Window
        {
        public:

            GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen = false);
            virtual ~GLWindow() = default;

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
        };
    }
}
