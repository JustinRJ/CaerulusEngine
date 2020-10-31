#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"

#include "Graphics/Pipeline/Shader.h"
#include "Graphics/Pipeline/Renderer.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

#include "Graphics/Geometry/Quad.h"
#include "Graphics/Geometry/Cube.h"
#include "Graphics/Geometry/Plane.h"
#include "Graphics/Geometry/Mesh.h"

#include "Graphics/Resource/Model.h"
#include "Graphics/Resource/Material.h"

#include "Graphics/Lighting/Light.h"
#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"

namespace
{
    using namespace Graphics::Light;
    using namespace Graphics::Window;
    using namespace Graphics::Resource;
    using namespace Graphics::Pipeline;
    using namespace Graphics::Geometry;
}

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(std::shared_ptr<GLWindow> window,
        std::shared_ptr<IRenderer> renderer) :
        m_renderer(renderer),
        m_window(window)
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

            for (std::shared_ptr<Model> model : m_models)
            {
                for (unsigned int i = 0; i < model->GetMeshes().size(); ++i)
                {
                    if (std::shared_ptr<Mesh> mesh = model->GetMeshes()[i])
                    {
                        std::shared_ptr<Material> material = model->GetMaterials()[i];

                        if (material)
                        {
                            material->Bind();

                            for (std::shared_ptr<Shader> shader : material->GetShaders())
                            {
                                if (shader)
                                {
                                    shader->Bind();
                                    shader->UpdateUniforms();
                                }
                            }
                        }

                        m_renderer->Draw(mesh->GetVertexArray(), mesh->GetIndexBuffer(), m_renderWireframe);

                        if (material)
                        {
                            material->Unbind();
                        }
                    }
                }
            }

            for (std::shared_ptr<Light::Light> light : m_lights)
            {
                if (std::shared_ptr<Shader> shader = light->GetShader())
                {
                    shader->Bind();
                    shader->UpdateUniforms();
                }
            }
        }

        m_window->SwapBuffer();
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

    void GraphicsEngine::SetLights(const std::vector<std::shared_ptr<Light::Light>>& lights)
    {
        m_lights = lights;
    }

    const std::vector<std::shared_ptr<Model>>& GraphicsEngine::GetModels() const
    {
        return m_models;
    }

    const std::vector<std::shared_ptr<Light::Light>>& GraphicsEngine::GetLights() const
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