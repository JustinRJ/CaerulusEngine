#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"
#include "Core/Math/Camera.h"

#include "Graphics/Pipeline/Shader.h"
#include "Graphics/PipeLine/Renderer.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

#include "Graphics/Geometry/Quad.h"
#include "Graphics/Geometry/Cube.h"
#include "Graphics/Geometry/Plane.h"
#include "Graphics/Resource/Model.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(std::shared_ptr<PipeLine::IRenderer> renderer) :
        m_renderer(renderer)
    {
    }

    void GraphicsEngine::PreUpdate(float deltaTime)
    {
        if (m_renderer)
        {
            m_renderer->Clear();
        }
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        if (m_renderer)
        {
            std::shared_ptr<Window::GLWindow> window = m_renderer->GetWindow();
            window->Update();

            m_renderer->GetCamera()->GetFrustrum().SetAspect(
                static_cast<float>(window->GetActiveState().Width) / static_cast<float>(window->GetActiveState().Height));

            // TODO - render all models, use static draw call with buffers in Mesh
            if (m_renderer->GetShader())
            {
                std::shared_ptr<Geometry::Quad> quad = std::make_shared<Geometry::Quad>();
                std::shared_ptr<Geometry::Plane> plane = std::make_shared<Geometry::Plane>();
                std::shared_ptr<Geometry::Cube> cube = std::make_shared<Geometry::Cube>();

                static std::shared_ptr<Resource::Model> shaderball;
                if (!shaderball)
                {
                    shaderball = std::make_shared<Resource::Model>("assets/models/shaderball.obj");
                }

                std::shared_ptr<Core::Math::Camera> camera = m_renderer->GetCamera();
                std::shared_ptr<PipeLine::Shader> shader = m_renderer->GetShader();

                mat4 model = glm::translate(mat4(1.0f), glm::vec3(0, 10, -5));
                mat4 mvp = camera->GetFrustrum().GetMatrix() * camera->GetTransform().GetMatrix() * model;

                shader->Bind();
                shader->Set4f("ourColor", fvec4(0.f, (sin(glfwGetTime()) / 2.0f) + 0.5f, 0.f, 1.0));
                shader->SetMat4fv("mvp", mvp);

                quad->Draw(m_renderWireframe);
                plane->Draw(m_renderWireframe);

                model = glm::translate(mat4(1.0f), glm::vec3(0, 9, -5));
                mvp = camera->GetFrustrum().GetMatrix() * camera->GetTransform().GetMatrix() * model;
                shader->SetMat4fv("mvp", mvp);

                cube->Draw(m_renderWireframe);

                model = glm::translate(mat4(1.0f), glm::vec3(0, 0, -5));
                mvp = camera->GetFrustrum().GetMatrix() * camera->GetTransform().GetMatrix() * model;
                shader->SetMat4fv("mvp", mvp);

                shaderball->Draw(m_renderWireframe, nullptr);
            }

            m_renderer->SwapBuffer(deltaTime);
        }
    }
}