#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/ITickable.h"
#include "Graphics/Resource/Model.h"

namespace Graphics
{
    namespace PipeLine
    {
        class IRenderer;
    }
}

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
    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(std::shared_ptr<PipeLine::IRenderer> renderer);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

        std::shared_ptr<PipeLine::IRenderer> GetRenderer() const
        {
            return m_renderer;
        }

        void SetModels(const std::vector<std::shared_ptr<Resource::Model>>& models)
        {
            m_models = models;
        }

        void ToggleWireframe()
        {
            m_renderWireframe = !m_renderWireframe;
        }

    private:
        bool m_renderWireframe = false;

        std::shared_ptr<PipeLine::IRenderer> m_renderer;

        std::vector<std::shared_ptr<Resource::Model>> m_models;
    };
}
