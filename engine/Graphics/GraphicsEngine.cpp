#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"
#include "Core/Math/Camera.h"

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

#include "Graphics/Lighting/PointLight.h"
#include "Graphics/Lighting/DirectionalLight.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(std::shared_ptr<Window::GLWindow> window,
        std::shared_ptr<Pipeline::IRenderer> renderer,
        std::shared_ptr<Core::Math::Camera> camera) :
        m_renderer(renderer),
        m_window(window),
        m_camera(camera)
    {
    }
    
    void GraphicsEngine::PreUpdate(float deltaTime)
    {
        m_renderer->Clear(m_clearColour);
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        m_window->Update();

        m_camera->GetFrustrum().SetAspect(
            static_cast<float>(m_window->GetActiveState().Width) / static_cast<float>(m_window->GetActiveState().Height));

        for (unsigned int i = 0; i < m_models.size(); ++i)
        {
            std::shared_ptr<Resource::Model> model = m_models[i];
            glm::mat4 modelTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -10)); // TODO - transform per model here
            modelTransform = glm::scale(modelTransform, glm::vec3(0.25, 0.25, 0.25));
            glm::mat4 mvp = m_camera->GetFrustrum().GetMatrix() * m_camera->GetTransform().GetMatrix() * modelTransform;

            const std::vector<std::shared_ptr<Resource::Material>>& materials = model->GetMaterials();

            //if (materials[i])
            //{
            //    materials[i]->Bind();
            //}

            // TODO - get shader out of material
            if (m_shader)
            {
                m_shader->Bind();
                m_shader->Set4f("ourColor", glm::fvec4(0.f, (sin(glfwGetTime()) / 2.0f) + 0.5f, 0.f, 1.0));
                m_shader->SetMat4fv("mvp", mvp);
            }

            for (std::shared_ptr<Geometry::Mesh> mesh : model->GetMeshes())
            {
                m_renderer->Draw(mesh->GetVertexArray(), mesh->GetIndexBuffer(), *m_shader, m_renderWireframe);
            }
        }

        m_window->SwapBuffer();
    }

    const glm::vec4& GraphicsEngine::GetClearColour() const
    {
        return m_clearColour;
    }

    void GraphicsEngine::SetClearColour(const glm::vec4& colour)
    {
        m_clearColour = colour;
    }

    std::shared_ptr<Window::GLWindow> GraphicsEngine::GetWindow() const
    {
        return m_window;
    }

    std::shared_ptr<Pipeline::Shader> GraphicsEngine::GetShader() const
    {
        return m_shader;
    }

    std::shared_ptr<Core::Math::Camera> GraphicsEngine::GetCamera() const
    {
        return m_camera;
    }

    void GraphicsEngine::SetWindow(std::shared_ptr<Window::GLWindow> window)
    {
        m_window = window;
    }

    void GraphicsEngine::SetShader(std::shared_ptr<Pipeline::Shader> shader)
    {
        m_shader = shader;
    }

    void GraphicsEngine::SetCamera(std::shared_ptr<Core::Math::Camera> camera)
    {
        m_camera = camera;
    }

    std::shared_ptr<Pipeline::IRenderer> GraphicsEngine::GetRenderer() const
    {
        return m_renderer;
    }

    void GraphicsEngine::SetRenderer(std::shared_ptr<Pipeline::IRenderer> renderer)
    {
        m_renderer = renderer;
    }

    void GraphicsEngine::SetModels(const std::vector<std::shared_ptr<Resource::Model>>& models)
    {
        m_models = models;
    }

    const std::vector<std::shared_ptr<Resource::Model>>& GraphicsEngine::GetModels() const
    {
        return m_models;
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