#include "stdafx.h"

#include "GraphicsEngine.h"
#include "Render/PhysicallyBasedRendering.h"
#include "../Core/Time/Timer.h"
#include "../Core/Math/MathHelper.h"
#include "../Core/Logging/Log.h"

namespace Graphics
{
    GraphicsEngine::GraphicsEngine(std::shared_ptr<GLWindow> window, std::shared_ptr<Camera> camera) :
        m_Window(window),
        m_Camera(camera)
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

        mat4 quadTransform;
        MathHelper::CreateTansform(quadTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
        m_Window->SetQuad(std::make_shared<Quad>(quadTransform));
    }

    void GraphicsEngine::PreUpdate(float deltaTime)
    {
        m_Renderer->FlushErrors();
        m_Renderer->Clear();
    }

    void GraphicsEngine::Update(float deltaTime)
    {
        m_Window->Update();
        m_Renderer->DrawAll(deltaTime);
        m_Renderer->SwapBuffer(deltaTime);
    }

    void GraphicsEngine::SetCamera(std::shared_ptr<Camera> camera)
    {
        m_Camera = camera;
    }

    std::shared_ptr<Camera> GraphicsEngine::GetCamera() const
    {
        return m_Camera;
    }

    void GraphicsEngine::ProfileGPUs()
    {
        GLuint64 startGeometryTime, startLightingTime, startSAOTime, startPostProcessTime, startForwardTime;
        GLuint64 stopGeometryTime, stopLightingTime, stopSAOTime, stopPostProcessTime, stopForwardTime;

        GLint stopGeometryTimerAvailable = 0;
        GLint stopLightingTimerAvailable = 0;
        GLint stopSAOTimerAvailable = 0;
        GLint stopPostprocessTimerAvailable = 0;
        GLint stopForwardTimerAvailable = 0;

        while (!stopGeometryTimerAvailable && !stopLightingTimerAvailable &&
            !stopSAOTimerAvailable && !stopPostprocessTimerAvailable && !stopForwardTimerAvailable)
        {
            glGetQueryObjectiv(m_Profiler.QueryIDGeometry[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopGeometryTimerAvailable);
            glGetQueryObjectiv(m_Profiler.QueryIDLighting[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopLightingTimerAvailable);
            glGetQueryObjectiv(m_Profiler.QueryIDSAO[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopSAOTimerAvailable);
            glGetQueryObjectiv(m_Profiler.QueryIDPostprocess[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopPostprocessTimerAvailable);
            glGetQueryObjectiv(m_Profiler.QueryIDForward[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopForwardTimerAvailable);
        }

        glGetQueryObjectui64v(m_Profiler.QueryIDGeometry[GPUProfiler::Start], GL_QUERY_RESULT, &startGeometryTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDGeometry[GPUProfiler::Stop], GL_QUERY_RESULT, &stopGeometryTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDLighting[GPUProfiler::Start], GL_QUERY_RESULT, &startLightingTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDLighting[GPUProfiler::Stop], GL_QUERY_RESULT, &stopLightingTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDSAO[GPUProfiler::Start], GL_QUERY_RESULT, &startSAOTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDSAO[GPUProfiler::Stop], GL_QUERY_RESULT, &stopSAOTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDPostprocess[GPUProfiler::Start], GL_QUERY_RESULT, &startPostProcessTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDPostprocess[GPUProfiler::Stop], GL_QUERY_RESULT, &stopPostProcessTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDForward[GPUProfiler::Start], GL_QUERY_RESULT, &startForwardTime);
        glGetQueryObjectui64v(m_Profiler.QueryIDForward[GPUProfiler::Stop], GL_QUERY_RESULT, &stopForwardTime);

        m_Profiler.DeltaGeometryTime    = (float)((stopGeometryTime       - startGeometryTime)    * Core::Time::MILLISECOND);
        m_Profiler.DeltaLightingTime    = (float)((stopLightingTime       - startLightingTime)    * Core::Time::MILLISECOND);
        m_Profiler.DeltaSAOTime         = (float)((stopSAOTime            - startSAOTime)         * Core::Time::MILLISECOND);
        m_Profiler.DeltaPostProcessTime = (float)((stopPostProcessTime    - startPostProcessTime) * Core::Time::MILLISECOND);
        m_Profiler.DeltaForwardTime     = (float)((stopForwardTime        - startForwardTime)     * Core::Time::MILLISECOND);
    }

    void GraphicsEngine::SetPointLightMap(std::map<unsigned int, std::shared_ptr<PointLight>> idPointMap)
    {
        m_PointLightMap = idPointMap;
    }

    std::map<unsigned int, std::shared_ptr<PointLight>>& GraphicsEngine::GetPointLightMap()
    {
        return m_PointLightMap;
    }

    void GraphicsEngine::SetDirectionalLightMap(std::map<unsigned int, std::shared_ptr<DirectionalLight>> idDirectionalMap)
    {
        m_DirectionalLightMap = idDirectionalMap;
    }

    std::map<unsigned int, std::shared_ptr<DirectionalLight>>& GraphicsEngine::GetDirectionalLightMap()
    {
        return m_DirectionalLightMap;
    }

    void GraphicsEngine::SetGLWindow(std::shared_ptr<GLWindow> window)
    {
        m_Window = window;
    }

    std::shared_ptr<GLWindow> GraphicsEngine::GetGLWindow() const
    {
        return m_Window;
    }

    void GraphicsEngine::SetModelMap(const std::map<unsigned int, std::shared_ptr<Model>>& modelMap)
    {
        m_ModelMap = modelMap;
    }

    const std::map<unsigned int, std::shared_ptr<Model>>& GraphicsEngine::GetModelMap() const
    {
        return m_ModelMap;
    }

    void GraphicsEngine::SetTransformMap(const std::map<unsigned int, std::shared_ptr<mat4>>& transformMap)
    {
        m_TransformMap = transformMap;
    }

    const std::map<unsigned int, std::shared_ptr<mat4>>& GraphicsEngine::GetTransformMap() const
    {
        return m_TransformMap;
    }

    void GraphicsEngine::SetRenderer(std::shared_ptr<Render::IRenderer> renderer)
    {
        m_Renderer = renderer;
    }

    std::shared_ptr<Render::IRenderer> GraphicsEngine::GetRenderer() const
    {
        return m_Renderer;
    }

    void GraphicsEngine::SetDefaultMaterial(std::shared_ptr<Material> material)
    {
        m_DefaultMaterial = material;
    }

    std::shared_ptr<Material> GraphicsEngine::GetDefaultMaterial() const
    {
        return m_DefaultMaterial;
    }
}