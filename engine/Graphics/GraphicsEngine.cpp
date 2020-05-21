#include "stdafx.h"

#include "GraphicsEngine.h"
#include "../Core/Time/Timer.h"
#include "../Core/Math/Math.h"
#include "../Core/Logging/Log.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(std::shared_ptr<GLWindow> window, std::shared_ptr<Camera> camera)
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