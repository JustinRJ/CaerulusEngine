#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Window.h"
#include "../Geometry/QuadGeometry.h"
#include <glfw3.h>

namespace Graphics
{
    namespace Window
    {
        using namespace Geometry;
        class CAERULUS_GRAPHICS GLWindow : public Window
        {
        public:

            GLWindow(const std::string& title, int x, int y, int bits, bool fullscreen = false);
            virtual ~GLWindow();

            virtual void Apply() override;
            virtual void Focus() override;
            virtual void SwapBuffer() const override;
            virtual void* GetHandle() const override;

            std::shared_ptr<GLFWwindow> GetGLFWWindow() const;

            void SetQuad(std::shared_ptr<QuadGeometry> quad);
            std::shared_ptr<QuadGeometry> GetQuad() const;
            
            void Update();
            void ToggleLockedCursor();
            bool IsCursorLocked() const;
            void CenterCursor();
        private:

            std::shared_ptr<GLFWwindow> m_Window;
            std::shared_ptr<QuadGeometry> m_Quad;
            bool m_LockedCursor;
        };
    }
}
