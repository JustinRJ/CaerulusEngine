#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Window.h"
#include <glew.h>
#include <glfw3.h>

namespace Graphics
{
    namespace Window
    {
        class CAERULUS_GRAPHICS GLWindow : public Window
        {
        public:

            GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen = false);
            virtual ~GLWindow();

            virtual void Apply() override;
            virtual void Focus() override;
            virtual void SwapBuffer() const override;
            virtual void* GetHandle() const override;

            virtual GLFWwindow* GetGLFWWindow() const;

            virtual void CenterCursor() const;
            virtual void ToggleLockedCursor();
        private:

            GLFWwindow* m_Window;
            bool m_LockedCursor;
        };
    }
}
