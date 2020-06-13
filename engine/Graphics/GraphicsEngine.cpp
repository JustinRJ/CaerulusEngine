#include "stdafx.h"

#include "GraphicsEngine.h"
#include "Core/Logging/Log.h"
#include "Window/GLWindow.h"
#include "Core/Math/Camera.h"
#include "Graphics/Render/VoxelRenderer.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(
        std::shared_ptr<Render::IRenderer> renderer,
        std::shared_ptr<Managers::ModelManager> modelManager,
        std::shared_ptr<Managers::ShaderManager> shaderManager) :
        m_renderer(renderer),
        m_modelManager(modelManager),
        m_shaderManager(shaderManager)
    {
        glewExperimental = true;

        using namespace Core::Logging;
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            Log::LogError("OpenGL Error", std::to_string(error));
            exit(1);
        }

        if (glewInit() != GLEW_OK)
        {
            Log::LogError("Failed to init GLEW!");
            exit(1);
        }
    }

    void GraphicsEngine::PreUpdate(float deltaTime)
    {
        if (m_renderer)
        {
            m_renderer->FlushErrors();
            m_renderer->Clear();
            m_renderer->SwapBuffer(deltaTime);
        }
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        if (m_renderer)
        {
            m_renderer->GetWindow()->Update();
            m_renderer->DrawAll(deltaTime);
        }
    }
}