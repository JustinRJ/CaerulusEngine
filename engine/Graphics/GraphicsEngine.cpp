#include "stdafx.h"

#include "GraphicsEngine.h"
#include "Core/Time/Timer.h"
#include "Core/Math/Math.h"
#include "Core/Logging/Log.h"

#include "Render/IRenderer.h"

#include "Window/GLWindow.h"

#include "PipeLine/Shader.h"

#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

#include "Geometry/Quad.h"
#include "Geometry/Cube.h"

#include "Resource/Model.h"
#include "Resource/Camera.h"
#include "Resource/Texture.h"
#include "Resource/Material.h"

namespace
{
    using namespace Graphics::Render;
    using namespace Graphics::Light;
    using namespace Graphics::Window;
    using namespace Graphics::PipeLine;
    using namespace Graphics::Geometry;
    using namespace Graphics::Resource;
}

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