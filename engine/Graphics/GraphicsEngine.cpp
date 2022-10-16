#include "stdafx.h"

#include "GraphicsEngine.h"

#include "Window/GLWindow.h"

#include "Pipeline/FrameBuffer.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"

#include "Rendering/GLRenderer.h"

#include "Geometry/Quad.h"
#include "Geometry/Cube.h"
#include "Geometry/Plane.h"
#include "Geometry/Mesh.h"
#include "Geometry/Model.h"
#include "Rendering/RenderInstance.h"

#include "Surface/Material.h"

#include "Lighting/IBL.h"
#include "Lighting/Light.h"
#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

#include "ECS/ManagerFactory.h"

namespace Graphics
{
    using namespace Window;
    using namespace Surface;
    using namespace Pipeline;
    using namespace Geometry;
    using namespace Lighting;
    using namespace Rendering;

    GraphicsEngine::GraphicsEngine(Core::ECS::ManagerFactory& managerFactory) :
        m_IBL(nullptr),
        m_window(nullptr),
        m_renderer(nullptr),
        m_framebuffer(*new Pipeline::FrameBuffer()),
        m_instanceManager(managerFactory.GetComponentManager<RenderInstance>().get()),
        m_pointLightManager(managerFactory.GetComponentManager<PointLight>().get())
    {}
    
    void GraphicsEngine::EarlyTick()
    {
        if (m_renderer)
        {
            m_renderer->Clear(m_clearColour);
        }

        if (m_instanceManager)
        {
            m_instanceManager->EarlyUpdate();
        }
        if (m_pointLightManager)
        {
            m_pointLightManager->EarlyUpdate();
        }
    }

    void GraphicsEngine::Tick(float deltaTime)
    {
        if (m_window)
        {
            m_window->Update();

            if (m_IBL)
            {
                m_IBL->Bind();
            }

            if (m_instanceManager)
            {
                m_instanceManager->Update(deltaTime);
            }
            if (m_pointLightManager)
            {
                m_pointLightManager->Update(deltaTime);
            }

            if (m_IBL)
            {
                m_IBL->InvokeUniformCallbacks();
            }
        }
    }

    void GraphicsEngine::FixedTick(float fixedTime)
    {
        if (m_instanceManager)
        {
            m_instanceManager->FixedUpdate(fixedTime);
        }
        if (m_pointLightManager)
        {
            m_pointLightManager->FixedUpdate(fixedTime);
        }
    }

    void GraphicsEngine::LateTick()
    {
        if (m_instanceManager)
        {
            m_instanceManager->LateUpdate();
        }
        if (m_pointLightManager)
        {
            m_pointLightManager->LateUpdate();
        }

        if (m_window)
        {
            m_window->SwapBuffer();
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

    GLWindow* GraphicsEngine::GetWindow()
    {
        return m_window;
    }

    void GraphicsEngine::SetWindow(GLWindow* window)
    {
        m_window = window;
        if (m_window)
        {
            m_framebuffer.Init(m_window->GetActiveState().Width, m_window->GetActiveState().Height, 8);
        }
    }

    IRenderer* GraphicsEngine::GetRenderer()
    {
        return m_renderer;
    }

    void GraphicsEngine::SetRenderer(IRenderer* renderer)
    {
        m_renderer = renderer;
    }

    Lighting::IBL* GraphicsEngine::GetIBL()
    {
        return m_IBL;
    }

    void GraphicsEngine::SetIBL(Lighting::IBL* ibl)
    {
        m_IBL = ibl;
    }

    Pipeline::FrameBuffer& GraphicsEngine::GetFrameBuffer()
    {
        return m_framebuffer;
    }
}