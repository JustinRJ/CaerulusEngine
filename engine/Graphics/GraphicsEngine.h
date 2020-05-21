#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <memory>
#include "Core/Interface/ITickable.h"

namespace Graphics
{
    namespace Window
    {
        class GLWindow;
    }
    namespace Resource
    {
        class Camera;
    }
}

namespace Graphics
{
    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(std::shared_ptr<Window::GLWindow> window, std::shared_ptr<Resource::Camera> camera);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}
    };
}
