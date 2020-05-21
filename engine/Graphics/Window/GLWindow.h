#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <glfw3.h>
#include "Window.h"

namespace Graphics
{
    namespace Geometry
    {
        class Quad;
    }
}

namespace Graphics
{
    namespace Window
    {
        using namespace Geometry;
        class CAERULUS_GRAPHICS GLWindow : public Window
        {
        public:

            GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen = false);
            virtual ~GLWindow() = default;

            void Apply() override;
            void Focus() override;
            void SwapBuffer() const override;
            void* GetHandle() const override;

            GLFWwindow* GetGLFWWindow() const;

            void SetQuad(std::shared_ptr<Quad> quad);
            std::shared_ptr<Quad> GetQuad() const;
            
            void Update();
            void ToggleLockedCursor();
            bool IsCursorLocked() const;
            void CenterCursor();
        private:

            GLFWwindow* m_window;
            std::shared_ptr<Quad> m_quad;
            bool m_lockedCursor;
        };
    }
}
