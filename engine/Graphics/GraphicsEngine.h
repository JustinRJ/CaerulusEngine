#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>

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

#include "../Core/Interface/ITickable.h"

namespace Graphics
{
    using namespace Render;
    using namespace Light;
    using namespace Window;
    using namespace PipeLine;
    using namespace Geometry;
    using namespace Resource;

    struct GPUProfiler
    {
        enum State
        {
            Start = 0,
            Stop = 1
        };

        GLfloat DeltaGeometryTime = 0.0f;
        GLfloat DeltaLightingTime = 0.0f;
        GLfloat DeltaSAOTime = 0.0f;
        GLfloat DeltaPostProcessTime = 0.0f;
        GLfloat DeltaForwardTime = 0.0f;

        unsigned int QueryIDGeometry[2];
        unsigned int QueryIDLighting[2];
        unsigned int QueryIDSAO[2];
        unsigned int QueryIDPostprocess[2];
        unsigned int QueryIDForward[2];

        void InitProfileQuery()
        {
            glGenQueries(2, QueryIDGeometry);
            glGenQueries(2, QueryIDLighting);
            glGenQueries(2, QueryIDSAO);
            glGenQueries(2, QueryIDPostprocess);
            glGenQueries(2, QueryIDForward);
        }
    };

    class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
    {
    public:

        GraphicsEngine(std::shared_ptr<GLWindow> window, std::shared_ptr<Camera> camera);

        void SetGLWindow(std::shared_ptr<GLWindow> window);
        std::shared_ptr<GLWindow> GetGLWindow() const;

        void SetCamera(std::shared_ptr<Camera> camera);
        std::shared_ptr<Camera>GetCamera() const;

        void SetDefaultMaterial(std::shared_ptr<Material> material);
        std::shared_ptr<Material> GetDefaultMaterial() const;

        void SetRenderer(std::shared_ptr<Render::IRenderer> renderer);
        std::shared_ptr<Render::IRenderer> GetRenderer() const;

        void PreUpdate(float deltaTime) override;
        void Update(float deltaTime) override;
        void FixedUpdate(float fixedTime) override {}
        void Reset() override {}

        void SetPointLightMap(std::map<unsigned int, std::shared_ptr<PointLight>> idPointMap);
        std::map<unsigned int, std::shared_ptr<PointLight>>& GetPointLightMap();

        void SetDirectionalLightMap(std::map<unsigned int, std::shared_ptr<DirectionalLight>> idDirectionalMap);
        std::map<unsigned int, std::shared_ptr<DirectionalLight>>& GetDirectionalLightMap();

        void SetModelMap(const std::map<unsigned int, std::shared_ptr<Model>>& modelMap);
        const std::map<unsigned int, std::shared_ptr<Model>>& GetModelMap() const;

        void SetTransformMap(const std::map<unsigned int, std::shared_ptr<mat4>>& transformMap);
        const std::map<unsigned int, std::shared_ptr<mat4>>& GetTransformMap() const;

    private:

        std::shared_ptr<Render::IRenderer> m_Renderer;

        //Misc
        void ProfileGPUs();

        std::shared_ptr<GLWindow> m_Window;

        std::shared_ptr<Camera> m_Camera;

        std::shared_ptr<Material> m_DefaultMaterial;

        GPUProfiler m_Profiler;

        // Map of ID to absolute transform
        std::map<unsigned int, std::shared_ptr<mat4>> m_TransformMap;
        // Map of ID to  model
        std::map<unsigned int, std::shared_ptr<Model>> m_ModelMap;
        // Map of ID to point light
        std::map<unsigned int, std::shared_ptr<PointLight>> m_PointLightMap;
        // Map of ID to directional light
        std::map<unsigned int, std::shared_ptr<DirectionalLight>> m_DirectionalLightMap;
    };
}

#pragma warning(push)