#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Window
    {
        class GLWindow;
    }
}

namespace Core
{
    namespace Math
    {
        class Camera;
    }
}

namespace Graphics
{
    namespace Render
    {
        class CAERULUS_GRAPHICS IRenderer : public Core::Interface::NonCopyable
        {
        public:

            virtual std::shared_ptr<Window::GLWindow> GetWindow() const = 0;
            virtual std::shared_ptr<Core::Math::Camera> GetCamera() const = 0;

            virtual void FlushErrors() = 0;
            virtual void Clear() = 0;
            virtual void DrawAll(float deltaTime) = 0;
            virtual void SwapBuffer(float deltaTime) = 0;
        };
    }
}