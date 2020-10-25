#pragma once

#include "IRenderer.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS Renderer : public IRenderer
        {
        public:
            Renderer();
            virtual ~Renderer() = default;

            void Clear() override;
            void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) override;
            void SwapBuffer(float deltaTime) override;

            glm::vec4 GetClearColour() const override;
            std::shared_ptr<Window::GLWindow> GetWindow() const override;
            std::shared_ptr<PipeLine::Shader> GetShader() const override;
            std::shared_ptr<Core::Math::Camera> GetCamera() const override;

            void SetClearColour(glm::vec4 colour) override;
            void SetWindow(std::shared_ptr<Window::GLWindow> window) override;
            void SetShader(std::shared_ptr<PipeLine::Shader> shader) override;
            void SetCamera(std::shared_ptr<Core::Math::Camera> camera) override;

        private:
            std::shared_ptr<Window::GLWindow> m_window;
            std::shared_ptr<PipeLine::Shader> m_shader;
            std::shared_ptr<Core::Math::Camera> m_camera;

            glm::vec4 m_clearColour = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
        };
    }
}