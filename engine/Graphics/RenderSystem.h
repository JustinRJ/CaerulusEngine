#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <map>

#include "Window/GLWindow.h"

#include "Shader/Shader.h"

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
        using namespace Shaders;
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
            GLint Samples;
            GLint Turns;
            GLint BlurSize;
            GLint MotionBlurMaxSamples;
            GLfloat Radius;
            GLfloat Bias;
            GLfloat Scale;
            GLfloat Contrast;
        };

        struct GPUProfiler
        {
            enum State
            {
                Start = 0,
                Stop = 1
            };

            GLfloat DeltaGeometryTime;
            GLfloat DeltaLightingTime;
            GLfloat DeltaSAOTime;
            GLfloat DeltaPostProcessTime;
            GLfloat DeltaForwardTime;

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

        class RenderSystem
        {
        public:

            CAERULUS_GRAPHICS RenderSystem();

            CAERULUS_GRAPHICS void Update(float deltaTime);

            CAERULUS_GRAPHICS void SetSkyBox(Texture& skyBox);
            CAERULUS_GRAPHICS Texture& GetSkyBox() const;

            CAERULUS_GRAPHICS void SetDefaultAO(Texture& ao);
            CAERULUS_GRAPHICS Texture& GetDefaultAO() const;

            CAERULUS_GRAPHICS void SetDefaultMaterial(Material& material);
            CAERULUS_GRAPHICS Material& GetDefaultMaterial() const;

            CAERULUS_GRAPHICS void SetGLWindow(GLWindow& window);
            CAERULUS_GRAPHICS GLWindow& GetGLWindow() const;

            CAERULUS_GRAPHICS void SetPointLightMap(std::map<unsigned int, Graphics::Light::PointLight*> idPointMap);
            CAERULUS_GRAPHICS const std::map<unsigned int, Graphics::Light::PointLight*>& GetPointLightMap() const;

            CAERULUS_GRAPHICS void SetDirectionalLightMap(std::map<unsigned int, Graphics::Light::DirectionalLight*> idDirectionalMap);
            CAERULUS_GRAPHICS const std::map<unsigned int, Graphics::Light::DirectionalLight*>& GetDirectionalLightMap() const;

            CAERULUS_GRAPHICS void SetTransformModelMap(const std::map<unsigned int, std::tuple<mat4*, Model*>>& transformModelMap);
            CAERULUS_GRAPHICS const std::map<unsigned int, std::tuple<mat4*, Model*>>& GetTransformModelMap() const;


            CAERULUS_GRAPHICS IBL& GetIBL() const;
            CAERULUS_GRAPHICS SAO& GetSAO() const;
            CAERULUS_GRAPHICS GPUProfiler& GetRenderProfiler() const;
            CAERULUS_GRAPHICS StandardShaders& GetShaders() const;

            CAERULUS_GRAPHICS void ToggleWireframe();
            CAERULUS_GRAPHICS void ToggleSAO();
            CAERULUS_GRAPHICS void ToggleFXAA();
            CAERULUS_GRAPHICS void ToggleMotionBlur();
            CAERULUS_GRAPHICS void TogglePointLightRender();
            CAERULUS_GRAPHICS void ToggleDirectionalLightRender();
            CAERULUS_GRAPHICS void ToggleEnviromentLightRender();
            CAERULUS_GRAPHICS void ToggleToneMapping(unsigned int tone = 1);

        private:

            //Initialize - in order
            bool InitGLEW() const;
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
            void SetNullMaterial();
            void GPUProfiling();

            GLWindow* m_Window;

            Camera* m_Camera;

            // Move to window class
            QuadGeometry*       m_WindowQuad;
            // Move to IBL
            Texture*            m_SkyBox;

            // Default AO
            Texture*            m_DefualtAO;
            // Default Material
            Material*           m_DefaultMaterial;
            // Map of ID to absolute transform + model
            std::map<unsigned int, std::tuple<mat4*, Model*>> m_TransformModelMap;
            // Map of ID to point light
            std::map<unsigned int, Graphics::Light::PointLight*> m_PointLightMap;
            // Map of ID to directional light
            std::map<unsigned int, Graphics::Light::DirectionalLight*> m_DirectionalLightMap;

            IBL* m_IBL;
            SAO* m_SAO;
            GPUProfiler* m_Profiler;
            StandardShaders* m_Shaders;

            bool m_PointMode;
            bool m_DirectionalMode;
            bool m_IBLMode;
            bool m_SAOMode;
            bool m_FXAAMode;
            bool m_MotionBlurMode;
            bool m_WireframeMode;
            bool m_ProfilingMode;

            GLint m_GBufferView;
            GLint m_AttenuationMode;
            GLint m_ToneMappingMode;

            // UE4 dielectric fresnel
            vec3 m_MaterialF0;

            //Render Settings
            GLfloat m_MaterialRoughness;
            GLfloat m_MaterialMetallicity;
            GLfloat m_AmbientIntensity;

            mat4 m_ProjViewModel;
            mat4 m_ProjViewModelPrev;

            //GBuffer
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
        };
    }
}