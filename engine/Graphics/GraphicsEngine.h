#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <memory>
#include <vector>

#include "Core/Math/Math.h"
#include "Core/Interface/ITickable.h"

namespace Graphics
{
    namespace Geometry
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
        class FrameBuffer;
        class ShaderUniformFunctor;
    }

    namespace Lighting
    {
        class IBL;
        class Light;
    }

    namespace Rendering
    {
        class IRenderer;
    }

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:
        GraphicsEngine(
            std::shared_ptr<Window::GLWindow> window,
            std::shared_ptr<Rendering::IRenderer> renderer);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

        std::shared_ptr<Pipeline::FrameBuffer> GetFrameBuffer() const;

        bool GetWireframe() const;
        const Core::Math::vec4& GetClearColour() const;
        std::shared_ptr<Window::GLWindow> GetWindow() const;
        std::shared_ptr<Rendering::IRenderer> GetRenderer() const;
        const std::vector<std::shared_ptr<Geometry::Model>>& GetModels() const;
        const std::vector<std::shared_ptr<Lighting::Light>>& GetLights() const;
        std::shared_ptr<Lighting::IBL> GetIBL() const;

        void SetWireframe(bool wireframe);
        void SetClearColour(const Core::Math::vec4& colour);
        void SetWindow(std::shared_ptr<Window::GLWindow> window);
        void SetRenderer(std::shared_ptr<Rendering::IRenderer> renderer);
        void SetModels(const std::vector<std::shared_ptr<Geometry::Model>>& models);
        void SetLights(const std::vector<std::shared_ptr<Lighting::Light>>& lights);
        void SetIBL(std::shared_ptr<Lighting::IBL> ibl);

    private:
        void UpdateModels();
        void UpdateLights();

        bool m_renderWireframe = false;
        Core::Math::vec4 m_clearColour = Core::Math::vec4(0.2f, 0.3f, 0.3f, 1.0f);

        std::shared_ptr<Lighting::IBL> m_IBL;
        std::shared_ptr<Window::GLWindow> m_window;
        std::shared_ptr<Rendering::IRenderer> m_renderer;
        std::shared_ptr<Pipeline::FrameBuffer> m_framebuffer;
        std::vector<std::shared_ptr<Geometry::Model>> m_models;
        std::vector<std::shared_ptr<Lighting::Light>> m_lights;
    };
}
