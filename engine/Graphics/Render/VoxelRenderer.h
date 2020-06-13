#pragma once

#include "IRenderer.h"

namespace Graphics
{
    namespace Render
    {
        class CAERULUS_GRAPHICS VoxelRenderer : public IRenderer
        {
        public:
            VoxelRenderer(std::shared_ptr<Window::GLWindow> window, std::shared_ptr<Core::Math::Camera> camera);
            virtual ~VoxelRenderer() = default;

            void FlushErrors() override {}
            void Clear() override {}
            void DrawAll(float deltaTime) override {}
            void SwapBuffer(float deltaTime) override {}

            std::shared_ptr<Window::GLWindow> GetWindow() const override;
            std::shared_ptr<Core::Math::Camera> GetCamera() const override;

        private:
            std::shared_ptr<Window::GLWindow> m_window;
            std::shared_ptr<Core::Math::Camera> m_camera;
        };
    }
}