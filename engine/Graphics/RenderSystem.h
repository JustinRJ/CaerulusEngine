#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>

#include "Window/GLWindow.h"

#include "PipeLine/Shader.h"

#include "Lighting/PointLight.h"
#include "Lighting/DirectionalLight.h"

#include "Geometry/QuadGeometry.h"
#include "Geometry/CubeGeometry.h"

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
            CubeGeometry SceneCube;
            mat4 MapProjection;
            std::vector<mat4> MapView;
        };

        struct SAO
        {
            GLint Samples = 12;
            GLint Turns = 7;
            GLint BlurSize = 4;
            GLfloat Radius = 0.3f;
            GLfloat Bias = 0.001f;
            GLfloat Scale = 0.7f;
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
        };

        struct StandardShaders
        {
            Shader GBuffer;
            Shader LatlongToCube;
            Shader Simple;
            Shader LightingBRDF;
            Shader IrradianceIBL;
            Shader PrefilterIBL;
            Shader IntegrateIBL;
            Shader FirstPassPostProcess;
            Shader SAO;
            Shader SAOBlur;
        };

        class CAERULUS_GRAPHICS RenderSystem : public Core::Interface::ITickable
        {
        public:

            RenderSystem(GLWindow& window);

            void Update(float deltaTime) override;
            void FixedUpdate(float fixedTime) override {};
            void LateUpdate(float deltaTime) override {};
            void Reset() override {};

            void SetCamera(Camera& camera);
            Camera& GetCamera();

            void SetSkyBox(Texture& skyBox);
            Texture& GetSkyBox();

            void SetGLWindow(GLWindow& window);
            GLWindow& GetGLWindow();

            void SetPointLightMap(std::map<unsigned int, Graphics::Light::PointLight*> idPointMap);
            const std::map<unsigned int, Graphics::Light::PointLight*>& GetPointLightMap() const;

            void SetDirectionalLightMap(std::map<unsigned int, Graphics::Light::DirectionalLight*> idDirectionalMap);
            const std::map<unsigned int, Graphics::Light::DirectionalLight*>& GetDirectionalLightMap() const;

            void SetModelMap(const std::map<unsigned int, Model*>& modelMap);
            const std::map<unsigned int, Model*>& GetModelMap() const;

            void SetTransformMap(const std::map<unsigned int, mat4*>& transformMap);
            const std::map<unsigned int, mat4*>& GetTransformMap() const;

            IBL& GetIBL() const;
            SAO& GetSAO() const;
            GPUProfiler& GetRenderProfiler() const;
            StandardShaders& GetShaders() const;

            void ToggleWireframe();
            void ToggleSAO();
            void ToggleFXAA();
            void ToggleMotionBlur();
            void TogglePointLightRender();
            void ToggleDirectionalLightRender();
            void ToggleEnviromentLightRender();
            void ToggleToneMapping(unsigned int tone = 1);

        private:

            //Initialize - in order
            void SetCapabilities() const;
            void LoadShaders();
            void SetShaderUniformLocations();
            void InitProfileQuery();

            //Setup FBOs
            void GBufferSetup();
            void SAOBufferSetup();
            void PostProcessBufferSetup();
            void IBLBufferSetup();

            //Update Calls
            void DrawAll();
            void Clear() const;
            void FlushErrors() const;
            void SwapBuffer(float frameRate);

            void SetCameraUniforms(const mat4& view, const mat4& proj, float nearPlane, float farPlane) const;
            void SetModelUniforms(const mat4& model) const;

            void SAORendering(const mat4& proj);
            void LightRendering(const mat4& view, const mat4& proj);
            void PostProcessRendering(GLfloat frameRate, GLfloat cameraAperture, GLfloat cameraShutterSpeed, GLfloat cameraISO);
            void ForwardPassRendering(const mat4& view, const mat4& proj);

            //Misc
            void UseNullMaterial();
            void ProfileGPUs();

            GLWindow& m_Window;

            Camera& m_Camera;

            GPUProfiler& m_Profiler;

            SAO& m_SAO;

            // Move to IBL
            Texture* m_SkyBox;

            IBL* m_IBL;
            StandardShaders* m_Shaders;

            bool m_PointMode;
            bool m_DirectionalMode;
            bool m_IBLMode;
            bool m_SAOMode;
            bool m_FXAAMode;
            bool m_MotionBlurMode;
            bool m_WireframeMode;
            bool m_ProfilingMode;

            //Render Settings
            GLfloat m_MaterialRoughness;
            GLfloat m_MaterialMetallicity;
            GLfloat m_AmbientIntensity;

            // Pipeline settings
            GLint m_GBufferView;
            GLint m_AttenuationMode;
            GLint m_ToneMappingMode;

            // GBuffer
            GLuint m_GBufferFBO;
            GLuint m_GPositionBuffer;
            GLuint m_GNormalBuffer;
            GLuint m_GAlbedoBuffer;
            GLuint m_GEffectsBuffer;

            //Depth/Z RBO
            GLuint m_DepthRBO;

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
            GLuint m_PrefilterRBO;

            // Map of ID to absolute transform
            std::map<unsigned int, Model*> m_ModelMap;
            // Map of ID to  model
            std::map<unsigned int, mat4*> m_TransformMap;
            // Map of ID to material
            std::map<unsigned int, Material*> m_MaterialMap;
            // Map of ID to point light
            std::map<unsigned int, Graphics::Light::PointLight*> m_PointLightMap;
            // Map of ID to directional light
            std::map<unsigned int, Graphics::Light::DirectionalLight*> m_DirectionalLightMap;

            // UE4 dielectric fresnel
            vec3 m_MaterialF0;

            mat4 m_ProjViewModel;
            mat4 m_ProjViewModelPrev;
        };
    }
}

#pragma warning(push)