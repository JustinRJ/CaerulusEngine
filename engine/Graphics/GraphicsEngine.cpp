#include "stdafx.h"

#include "GraphicsEngine.h"
#include "Core/Logging/Log.h"
#include "Window/GLWindow.h"
#include "Core/Math/Camera.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(
        std::shared_ptr<Window::GLWindow> window,
        std::shared_ptr<Core::Math::Camera> camera,
        std::shared_ptr<Managers::ModelManager> modelManager,
        std::shared_ptr<Managers::ShaderManager> shaderManager)
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
    }

    void GraphicsEngine::Update(float deltaTime)
    {
    }
}