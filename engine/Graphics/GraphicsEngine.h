#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <memory>
#include "Core/Interface/ITickable.h"

namespace Graphics
{
    namespace Render
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

        GraphicsEngine(
            std::shared_ptr<Render::IRenderer> renderer,
            std::shared_ptr<Managers::ModelManager> modelManager,
            std::shared_ptr<Managers::ShaderManager> shaderManager);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

    private:
        std::shared_ptr<Render::IRenderer> m_renderer;
        std::shared_ptr<Managers::ModelManager> m_modelManager;
        std::shared_ptr<Managers::ShaderManager> m_shaderManager;
    };
}
