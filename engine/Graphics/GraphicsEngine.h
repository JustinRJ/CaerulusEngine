#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>

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
    namespace Render
    {
        using namespace Light;
        using namespace Window;
        using namespace PipeLine;
        using namespace Geometry;
        using namespace Resource;

        struct IBL
        {
            Texture MapCube;
            Texture MapIrradiance;
            Texture MapPrefilter;
            Texture MapLUT;
            Cube SceneCube;
            mat4 MapProjection;
            std::vector<mat4> MapView;
        };

        struct SAO
        {
            GLint Turns = 7;
            GLint Samples = 12;
            GLint BlurSize = 4;
            GLfloat Bias = 0.001f;
            GLfloat Scale = 0.7f;
            GLfloat Radius = 0.3f;
            GLfloat Contrast = 0.8f;
            GLint MotionBlurMaxSamples = 32;
        };

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

        struct StandardShaders
        {
            std::shared_ptr<Shader> GBuffer;
            std::shared_ptr<Shader> LatlongToCube;
            std::shared_ptr<Shader> Simple;
            std::shared_ptr<Shader> LightingBRDF;
            std::shared_ptr<Shader> IrradianceIBL;
            std::shared_ptr<Shader> PrefilterIBL;
            std::shared_ptr<Shader> IntegrateIBL;
            std::shared_ptr<Shader> FirstPassPostProcess;
            std::shared_ptr<Shader> SAO;
            std::shared_ptr<Shader> SAOBlur;
        };

        class CAERULUS_GRAPHICS GraphicsEngine : public Core::Interface::ITickable
        {
        public:

            GraphicsEngine(std::shared_ptr<GLWindow> window, std::shared_ptr<Camera> camera);

            void SetGLWindow(std::shared_ptr<GLWindow> window);
            std::shared_ptr<GLWindow> GetGLWindow();

            void SetCamera(std::shared_ptr<Camera> camera);
            std::shared_ptr<Camera>GetCamera();

            void SetSkyBox(std::shared_ptr<Texture> skyBox);
            std::shared_ptr<Texture> GetSkyBox() const;

            void PreUpdate(float deltaTime) override;
            void Update(float deltaTime) override;
            void FixedUpdate(float fixedTime) override {}
            void Reset() override {}

            void SetPointLightMap(std::map<unsigned int, std::shared_ptr<PointLight>> idPointMap);
            const std::map<unsigned int, std::shared_ptr<PointLight>>& GetPointLightMap() const;

            void SetDirectionalLightMap(std::map<unsigned int, std::shared_ptr<DirectionalLight>> idDirectionalMap);
            const std::map<unsigned int, std::shared_ptr<DirectionalLight>>& GetDirectionalLightMap() const;

            void SetDefaultMaterial(std::shared_ptr<Material> material);
            std::shared_ptr<Material> GetDefaultMaterial() const;

            void SetModelMap(const std::map<unsigned int, std::shared_ptr<Model>>& modelMap);
            const std::map<unsigned int, std::shared_ptr<Model>>& GetModelMap() const;

            void SetTransformMap(const std::map<unsigned int, std::shared_ptr<mat4>>& transformMap);
            const std::map<unsigned int, std::shared_ptr<mat4>>& GetTransformMap() const;

            void SetShaders(const std::shared_ptr<StandardShaders>& shaders);
            const std::shared_ptr<StandardShaders>& GetShaders() const;

            void SetGBufferFBO(GLuint fbo);


            void ToggleWireframe();
            void ToggleSAO();
            void ToggleFXAA();
            void ToggleMotionBlur();
            void TogglePointLightRender();
            void ToggleDirectionalLightRender();
            void ToggleEnviromentLightRender();
            void ToggleToneMapping(unsigned int tone = 1);

            //TODO - remove once shaders automatically load
            void LoadShaders();

        private:

            //Update Calls
            void DrawAll();
            void Clear() const;
            void FlushErrors() const;
            void SwapBuffer(float deltaTime);

            void SAORendering(const mat4& proj);
            void LightRendering(const mat4& view, const mat4& proj);
            void PostProcessRendering(GLfloat deltaTime);
            void ForwardPassRendering(const mat4& view, const mat4& proj);

            //Misc
            void ProfileGPUs();

            std::shared_ptr<GLWindow> m_Window;

            std::shared_ptr<Camera> m_Camera;

            std::shared_ptr<Material> m_DefaultMaterial;

            GPUProfiler m_Profiler;

            bool m_PointMode;
            bool m_DirectionalMode;
            bool m_IBLMode;
            bool m_SAOMode;
            bool m_FXAAMode;
            bool m_MotionBlurMode;
            bool m_WireframeMode;
            bool m_ProfilingMode;

            // Map of ID to absolute transform
            std::map<unsigned int, std::shared_ptr<mat4>> m_TransformMap;
            // Map of ID to  model
            std::map<unsigned int, std::shared_ptr<Model>> m_ModelMap;
            // Map of ID to point light
            std::map<unsigned int, std::shared_ptr<PointLight>> m_PointLightMap;
            // Map of ID to directional light
            std::map<unsigned int, std::shared_ptr<DirectionalLight>> m_DirectionalLightMap;


            /// Move to pipeline system
            ///-----------------------------------------------------------------------------------

            //Render Settings
            GLfloat m_MaterialRoughness;
            GLfloat m_MaterialMetallicity;
            GLfloat m_AmbientIntensity;

            // Pipeline settings
            GLint m_GBufferView;
            GLint m_AttenuationMode;
            GLint m_ToneMappingMode;

            std::shared_ptr<IBL> m_IBL;

            std::shared_ptr<SAO> m_SAO;

            // Move to IBL
            std::shared_ptr<Texture> m_SkyBox;

            std::shared_ptr<StandardShaders> m_Shaders;

            // GBuffer
            GLuint m_GBufferFBO;
            GLuint m_DepthRBO;
            GLuint m_GPositionBuffer;
            GLuint m_GNormalBuffer;
            GLuint m_GAlbedoBuffer;
            GLuint m_GEffectsBuffer;

            //SAO FBO
            GLuint m_SAOFBO;
            GLuint m_SAOBuffer;

            //SAO Blur FBO
            GLuint m_SAOBlurFBO;
            GLuint m_SAOBlurBuffer;

            //Post Process FBO
            GLuint m_PostProcessFBO;
            GLuint m_PostProcessBuffer;

            //Cube FBO/RBO
            GLuint m_EnvToCubeFBO;
            GLuint m_EnvToCubeRBO;

            //IBL FBO/RBO
            GLuint m_IrradianceFBO;
            GLuint m_IrradianceRBO;

            //BRDF FBO/RBO
            GLuint m_BrdfLUTFBO;
            GLuint m_BrdfLUTRBO;

            //Prefilter FBO/RBO
            GLuint m_PrefilterFBO;

            // UE4 dielectric fresnel
            vec3 m_MaterialF0;

            mat4 m_ProjViewModel;
            mat4 m_ProjViewModelPrev;

            bool gBuffer = false;
            bool lightingBRDF = false;
            bool sao = false;
            bool firstPassProcess = false;
        };
    }
}

#pragma warning(push)