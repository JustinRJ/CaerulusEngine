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
    using namespace Graphics::Window;
    using namespace Graphics::Resource;
    using namespace Graphics::Pipeline;
    using namespace Graphics::Geometry;
    using namespace Graphics::Lighting;
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

            UpdateShaderUniforms(ShaderPriority::PreProcess);
            UpdateModels();
            UpdateShaderUniforms(ShaderPriority::MidProcess);
            UpdateLights();
            UpdateShaderUniforms(ShaderPriority::PostProcess);

            m_window->SwapBuffer();
        }
    }

    void GraphicsEngine::UpdateModels()
    {
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
    }

    void GraphicsEngine::UpdateLights()
    {
        for (std::shared_ptr<Light> light : m_lights)
        {
            if (std::shared_ptr<Shader> shader = light->GetShader())
            {
                shader->UpdateUniforms();
            }
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

    void GraphicsEngine::SetShaders(const std::map<ShaderPriority, std::vector<std::shared_ptr<Pipeline::Shader>>>& shaders)
    {
        m_shaders = shaders;
    }

    void GraphicsEngine::UpdateShaderUniforms(ShaderPriority priority)
    {
        if (m_shaders.find(priority) != m_shaders.end())
        {
            for (std::shared_ptr<Shader> shader : m_shaders.at(priority))
            {
                if (shader)
                {
                    shader->UpdateUniforms();
                }
            }
        }
    }

    const std::vector<std::shared_ptr<Model>>& GraphicsEngine::GetModels() const
    {
        return m_models;
    }

    const std::vector<std::shared_ptr<Light>>& GraphicsEngine::GetLights() const
    {
        return m_lights;
    }

    const std::map<ShaderPriority, std::vector<std::shared_ptr<Pipeline::Shader>>>& GraphicsEngine::GetShaders() const
    {
        return m_shaders;
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