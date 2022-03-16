#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"

#include "Pipeline/FrameBuffer.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"

#include "Rendering/GLRenderer.h"

#include "Geometry/Quad.h"
#include "Geometry/Cube.h"
#include "Geometry/Plane.h"
#include "Geometry/Mesh.h"
#include "Geometry/Model.h"

#include "Surface/Material.h"

#include "Lighting/IBL.h"
#include "Lighting/Light.h"
#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

namespace Graphics
{
    using namespace Window;
    using namespace Surface;
    using namespace Pipeline;
    using namespace Geometry;
    using namespace Lighting;
    using namespace Rendering;

    GraphicsEngine::GraphicsEngine(
        const Managers::ModelManager& modelManager,
        const Managers::PointLightManager& pointLightManager,
        std::shared_ptr<GLWindow> window,
        std::shared_ptr<IRenderer> renderer) :
        m_window(window),
        m_renderer(renderer),
        m_modelManager(modelManager),
        m_pointLightManager(pointLightManager)
    {
        m_framebuffer = std::make_shared<Pipeline::FrameBuffer>();
        m_framebuffer->Init(m_window->GetActiveState().Width, m_window->GetActiveState().Height, 8);
    }
    
    void GraphicsEngine::EarlyUpdate()
    {
        m_renderer->Clear(m_clearColour);
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        if (m_window)
        {
            m_window->Update();

            // TODO - setup the engine/component update loops to avoid this

            // TODO - manually calling m_IBL bind is bad
            // add functionality to ShaderUniform to allow priority of update to be set
            if (m_IBL)
            {
                m_IBL->Bind();
            }

            if (m_renderer)
            {
                UpdateModels();
            }

            UpdateLights();

            if (m_IBL)
            {
                m_IBL->InvokeUniformCallbacks();
            }

            m_window->SwapBuffer();
        }
    }

    void GraphicsEngine::UpdateModels()
    {
        for (auto model : m_modelManager.GetAll())
        {
            if (model)
            {
                model->InvokeUniformCallbacks();
                for (const std::shared_ptr<Mesh>& mesh : model->GetMeshes())
                {
                    if (mesh)
                    {
                        mesh->InvokeUniformCallbacks();
                        if (const Material* material = mesh->GetMaterial())
                        {
                            material->Bind();
                            material->InvokeUniformCallbacks();
                            m_renderer->Draw(*mesh, m_renderWireframe);
                        }
                    }
                }
            }
        }
    }

    void GraphicsEngine::UpdateLights()
    {
        for (const PointLight* light : m_pointLightManager.GetAll())
        {
            light->InvokeUniformCallbacks();
        }
    }

    const Core::Math::vec4& GraphicsEngine::GetClearColour() const
    {
        return m_clearColour;
    }

    void GraphicsEngine::SetClearColour(const Core::Math::vec4& colour)
    {
        m_clearColour = colour;
    }

    std::shared_ptr<GLWindow> GraphicsEngine::GetWindow() const
    {
        return m_window;
    }

    void GraphicsEngine::SetWindow(std::shared_ptr<GLWindow> window)
    {
        m_window = window;
    }

    std::shared_ptr<IRenderer> GraphicsEngine::GetRenderer() const
    {
        return m_renderer;
    }

    void GraphicsEngine::SetRenderer(std::shared_ptr<IRenderer> renderer)
    {
        m_renderer = renderer;
    }

    std::shared_ptr<Lighting::IBL> GraphicsEngine::GetIBL() const
    {
        return m_IBL;
    }

    void GraphicsEngine::SetWireframe(bool wireframe)
    {
        m_renderWireframe = wireframe;
    }

    bool GraphicsEngine::GetWireframe() const
    {
        return m_renderWireframe;
    }

    void GraphicsEngine::SetIBL(std::shared_ptr<Lighting::IBL> ibl)
    {
        m_IBL = ibl;
    }

    std::shared_ptr<Pipeline::FrameBuffer> GraphicsEngine::GetFrameBuffer() const
    {
        return m_framebuffer;
    }
}