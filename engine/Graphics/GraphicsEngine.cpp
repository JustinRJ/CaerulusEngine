#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"

#include "Graphics/Pipeline/GLRenderer.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

#include "Graphics/Geometry/Quad.h"
#include "Graphics/Geometry/Cube.h"
#include "Graphics/Geometry/Plane.h"
#include "Graphics/Geometry/Mesh.h"
#include "Graphics/Geometry/Model.h"

#include "Graphics/Surface/Material.h"

#include "Graphics/Lighting/Light.h"
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"

namespace Graphics
{
    using namespace Window;
    using namespace Surface;
    using namespace Pipeline;
    using namespace Geometry;
    using namespace Lighting;

    GraphicsEngine::GraphicsEngine(std::shared_ptr<GLWindow> window,
        std::shared_ptr<IRenderer> renderer) :
        m_window(window),
        m_renderer(renderer)
    {
    }
    
    void GraphicsEngine::PreUpdate(float deltaTime)
    {
        m_renderer->Clear(m_clearColour);
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        if (m_window)
        {
            m_window->Update();
            UpdateModels();
            UpdateLights();
            m_window->SwapBuffer();
        }
    }

    void GraphicsEngine::UpdateModels()
    {
        for (std::shared_ptr<Geometry::Model> model : m_models)
        {
            if (model)
            {
                model->InvokeUniformFunctors();
                for (std::shared_ptr<Mesh> mesh : model->GetMeshes())
                {
                    if (mesh)
                    {
                        mesh->InvokeUniformFunctors();
                        if (std::shared_ptr<Material> material = mesh->GetMaterial())
                        {
                            material->Bind();
                            material->InvokeUniformFunctors();

                            m_renderer->Draw(mesh->GetVertexArray(), mesh->GetIndexBuffer(), m_renderWireframe);
                        }
                    }
                }
            }
        }
    }

    void GraphicsEngine::UpdateLights()
    {
        for (std::shared_ptr<Light> light : m_lights)
        {
            light->InvokeUniformFunctors();
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

    void GraphicsEngine::SetModels(const std::vector<std::shared_ptr<Model>>& models)
    {
        m_models = models;
    }

    void GraphicsEngine::SetLights(const std::vector<std::shared_ptr<Light>>& lights)
    {
        m_lights = lights;
    }

    const std::vector<std::shared_ptr<Model>>& GraphicsEngine::GetModels() const
    {
        return m_models;
    }

    const std::vector<std::shared_ptr<Light>>& GraphicsEngine::GetLights() const
    {
        return m_lights;
    }

    void GraphicsEngine::SetWireframe(bool wireframe)
    {
        m_renderWireframe = wireframe;
    }

    bool GraphicsEngine::GetWireframe() const
    {
        return m_renderWireframe;
    }
}