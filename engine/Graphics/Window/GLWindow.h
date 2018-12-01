#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Window.h"

#include <glfw3.h>

namespace Graphics
{
    namespace Window
    {
        class GLWindow : public Window
        {
        public:

            CAERULUS_GRAPHICS GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen = false);
            CAERULUS_GRAPHICS virtual ~GLWindow();

            CAERULUS_GRAPHICS virtual void Apply() override;
            CAERULUS_GRAPHICS virtual void Focus() override;
            CAERULUS_GRAPHICS virtual void SwapBuffer() const override;
            CAERULUS_GRAPHICS virtual void* GetHandle() const override;

            CAERULUS_GRAPHICS virtual GLFWwindow* GetWindow() const;
        private:

            GLFWwindow* m_Window;
        };
    }
}
