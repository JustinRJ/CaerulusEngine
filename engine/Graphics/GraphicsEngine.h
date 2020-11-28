#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

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
        class IRenderer;
        class ShaderUniformFunctor;
    }

    namespace Lighting
    {
        class Light;
    }

    enum class PipelineProcess
    {
        PreProcess,
        MidProcess,
        PostProcess
    };

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(
            std::shared_ptr<Window::GLWindow> window,
            std::shared_ptr<Pipeline::IRenderer> renderer);

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

        bool GetWireframe() const;
        const Core::Math::vec4& GetClearColour() const;
        std::shared_ptr<Window::GLWindow> GetWindow() const;
        std::shared_ptr<Pipeline::IRenderer> GetRenderer() const;
        const std::vector<std::shared_ptr<Geometry::Model>>& GetModels() const;
        const std::vector<std::shared_ptr<Lighting::Light>>& GetLights() const;

        void SetWireframe(bool wireframe);
        void SetClearColour(const Core::Math::vec4& colour);
        void SetWindow(std::shared_ptr<Window::GLWindow> window);
        void SetRenderer(std::shared_ptr<Pipeline::IRenderer> renderer);
        void SetModels(const std::vector<std::shared_ptr<Geometry::Model>>& models);
        void SetLights(const std::vector<std::shared_ptr<Lighting::Light>>& lights);

        void SetProcessUniformFunctor(PipelineProcess process, std::shared_ptr<Pipeline::ShaderUniformFunctor> functor);

    private:
        void UpdateModels();
        void UpdateLights();

        void InvokePipelineProcessFunctors(PipelineProcess process) const;

        bool m_renderWireframe = false;
        Core::Math::vec4 m_clearColour = Core::Math::vec4(0.2f, 0.3f, 0.3f, 1.0f);

        std::shared_ptr<Window::GLWindow> m_window;
        std::shared_ptr<Pipeline::IRenderer> m_renderer;

        std::vector<std::shared_ptr<Geometry::Model>> m_models;
        std::vector<std::shared_ptr<Lighting::Light>> m_lights;

        std::map<PipelineProcess, std::shared_ptr<Pipeline::ShaderUniformFunctor>> m_shaderProcessFunctors;
    };
}
