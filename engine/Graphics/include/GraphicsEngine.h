#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <memory>
#include <vector>

#include "Math/Math.h"
#include "Interface/ITickable.h"
#include "ComponentManagers/ModelManager.h"
#include "ComponentManagers/PointLightManager.h"

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
        class ShaderUniformCallback;
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
            const Managers::ModelManager& modelManager,
            const Managers::PointLightManager& pointLightManager,
            std::shared_ptr<Window::GLWindow> window,
            std::shared_ptr<Rendering::IRenderer> renderer);

        void EarlyUpdate() override;
        void Update(float deltaTime) override;

        std::shared_ptr<Pipeline::FrameBuffer> GetFrameBuffer() const;

        bool GetWireframe() const;
        const Core::Math::vec4& GetClearColour() const;
        std::shared_ptr<Window::GLWindow> GetWindow() const;
        std::shared_ptr<Rendering::IRenderer> GetRenderer() const;
        std::shared_ptr<Lighting::IBL> GetIBL() const;

        void SetWireframe(bool wireframe);
        void SetClearColour(const Core::Math::vec4& colour);
        void SetWindow(std::shared_ptr<Window::GLWindow> window);
        void SetRenderer(std::shared_ptr<Rendering::IRenderer> renderer);
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

        const Managers::ModelManager& m_modelManager;
        const Managers::PointLightManager& m_pointLightManager;
    };
}
