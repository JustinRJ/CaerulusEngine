#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

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
        class GLWindow;
    }

    namespace PipeLine
    {
        class Shader;
        class VertexArray;
        class IndexBuffer;

        class CAERULUS_GRAPHICS IRenderer : public Core::Interface::NonCopyable
        {
        public:

            virtual void Clear() = 0;
            virtual void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) = 0;
            virtual void SwapBuffer(float deltaTime) = 0;

            virtual glm::vec4 GetClearColour() const = 0;
            virtual std::shared_ptr<Window::GLWindow> GetWindow() const = 0;
            virtual std::shared_ptr<PipeLine::Shader> GetShader() const = 0;
            virtual std::shared_ptr<Core::Math::Camera> GetCamera() const = 0;

            virtual void SetClearColour(glm::vec4 colour) = 0;
            virtual void SetWindow(std::shared_ptr<Window::GLWindow> window) = 0;
            virtual void SetShader(std::shared_ptr<PipeLine::Shader> shader) = 0;
            virtual void SetCamera(std::shared_ptr<Core::Math::Camera> camera) = 0;
        };
    }
}