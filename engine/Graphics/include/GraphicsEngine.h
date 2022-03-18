#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <memory>
#include <vector>

#include "Math/Math.h"
#include "Interface/ITickable.h"

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

    namespace ComponentManagers
    {
        class ModelManager;
        class PointLightManager;
    }

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:
        GraphicsEngine(
            ComponentManagers::ModelManager& modelManager,
            ComponentManagers::PointLightManager& pointLightManager);

        void EarlyTick() override;
        void Tick(float deltaTime) override;
        void FixedTick(float fixedTime) override;
        void LateTick() override;

        Pipeline::FrameBuffer& GetFrameBuffer();

        Lighting::IBL* GetIBL();
        void SetIBL(Lighting::IBL* ibl);

        void SetWindow(Window::GLWindow* window);
        Window::GLWindow* GetWindow();

        void SetClearColour(const Core::Math::vec4& colour);
        const Core::Math::vec4& GetClearColour() const;

        void SetRenderer(Rendering::IRenderer* renderer);
        Rendering::IRenderer* GetRenderer();

    private:
        Core::Math::vec4 m_clearColour = Core::Math::vec4(0.2f, 0.3f, 0.3f, 1.0f);

        Lighting::IBL* m_IBL;
        Window::GLWindow* m_window;
        Rendering::IRenderer* m_renderer;

        Pipeline::FrameBuffer& m_framebuffer;

        ComponentManagers::ModelManager* m_modelManager;
        ComponentManagers::PointLightManager* m_pointLightManager;
    };
}
