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

#include "Surface/Material.h"

#include "Lighting/IBL.h"
#include "Lighting/Light.h"
#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

namespace Graphics
{
    using namespace Window;
    using namespace Surface;
    using namespace Pipeline;
    using namespace Geometry;
    using namespace Lighting;
    using namespace Rendering;

    GraphicsEngine::GraphicsEngine(
        Managers::ModelManager& modelManager,
        Managers::PointLightManager& pointLightManager,
        std::shared_ptr<GLWindow> window,
        std::shared_ptr<IRenderer> renderer) :
        m_window(window),
        m_renderer(renderer),
        m_modelManager(modelManager),
        m_pointLightManager(pointLightManager)
    {
        m_framebuffer = std::make_shared<Pipeline::FrameBuffer>();
        m_framebuffer->Init(m_window->GetActiveState().Width, m_window->GetActiveState().Height, 8);
    }
    
    void GraphicsEngine::EarlyTick()
    {
        m_renderer->Clear(m_clearColour);

        m_modelManager.EarlyUpdate();
        m_pointLightManager.EarlyUpdate();
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

            m_modelManager.Update(deltaTime);
            m_pointLightManager.Update(deltaTime);

            if (m_IBL)
            {
                m_IBL->InvokeUniformCallbacks();
            }
        }
    }

    void GraphicsEngine::FixedTick(float fixedTime)
    {
        m_modelManager.FixedUpdate(fixedTime);
        m_pointLightManager.FixedUpdate(fixedTime);
    }

    void GraphicsEngine::LateTick()
    {
        m_modelManager.LateUpdate();
        m_pointLightManager.LateUpdate();

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

    std::shared_ptr<Lighting::IBL> GraphicsEngine::GetIBL() const
    {
        return m_IBL;
    }

    void GraphicsEngine::SetIBL(std::shared_ptr<Lighting::IBL> ibl)
    {
        m_IBL = ibl;
    }

    std::shared_ptr<Pipeline::FrameBuffer> GraphicsEngine::GetFrameBuffer() const
    {
        return m_framebuffer;
    }
}