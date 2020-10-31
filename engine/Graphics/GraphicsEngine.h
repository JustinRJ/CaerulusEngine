#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/ITickable.h"

namespace Core
{
    namespace Math
    {
        class Camera;
    }
}

namespace Managers
{
    class ModelManager;
    class ShaderManager;
}

namespace Graphics
{
    namespace Resource
    {
        class Model;
    }

    namespace Window
    {
        class GLWindow;
    }

    namespace Pipeline
    {
        class Shader;
        class IRenderer;
    }

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(
            std::shared_ptr<Window::GLWindow> window,
            std::shared_ptr<Pipeline::IRenderer> renderer,
            std::shared_ptr<Core::Math::Camera> camera);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

        bool GetWireframe() const;
        const glm::vec4& GetClearColour() const;
        std::shared_ptr<Pipeline::IRenderer> GetRenderer() const;
        std::shared_ptr<Window::GLWindow> GetWindow() const;
        std::shared_ptr<Pipeline::Shader> GetShader() const;
        std::shared_ptr<Core::Math::Camera> GetCamera() const;
        const std::vector<std::shared_ptr<Resource::Model>>& GetModels() const;

        void SetWireframe(bool wireframe);
        void SetClearColour(const glm::vec4& colour);
        void SetRenderer(std::shared_ptr<Pipeline::IRenderer> renderer);
        void SetWindow(std::shared_ptr<Window::GLWindow> window);
        void SetShader(std::shared_ptr<Pipeline::Shader> shader);
        void SetCamera(std::shared_ptr<Core::Math::Camera> camera);
        void SetModels(const std::vector<std::shared_ptr<Resource::Model>>& models);

    private:
        bool m_renderWireframe = false;
        glm::vec4 m_clearColour = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);

        std::shared_ptr<Pipeline::IRenderer> m_renderer;
        std::shared_ptr<Window::GLWindow> m_window;
        std::shared_ptr<Pipeline::Shader> m_shader;
        std::shared_ptr<Core::Math::Camera> m_camera;;

        std::vector<std::shared_ptr<Resource::Model>> m_models;
    };
}
