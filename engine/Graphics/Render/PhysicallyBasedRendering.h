#pragma once

#include "IRenderer.h"

#include "../Window/GLWindow.h"

#include "../PipeLine/Shader.h"

#include "../Lighting/PointLight.h"
#include "../Lighting/DirectionalLight.h"

#include "../Geometry/Quad.h"
#include "../Geometry/Cube.h"

#include "../Resource/Model.h"
#include "../Resource/Camera.h"
#include "../Resource/Texture.h"
#include "../Resource/Material.h"

namespace Managers
{
    class ShaderManager;
}

namespace Graphics
{
    class GraphicsEngine;

    using namespace Light;
    using namespace Window;
    using namespace PipeLine;
    using namespace Geometry;
    using namespace Resource;
    using namespace Managers;

    namespace Render
    {
        class CAERULUS_GRAPHICS PhysicallyBasedRendering : public IRenderer
        {
        public:

            PhysicallyBasedRendering(GraphicsEngine& graphicsEngine);

            void Init();

            void FlushErrors() override;
            void Clear() override;
            void DrawAll(float deltaTime) override;
            void SwapBuffer(float deltaTime) override;

            void ToggleWireframe();
            void ToggleSAO();
            void ToggleFXAA();
            void ToggleMotionBlur();
            void TogglePointLights();
            void ToggleDirectionalLights();
            void ToggleEnviromentLights();
            void ToggleToneMapping(unsigned int tone = 1);

            ///Material/Light Settings
            GLfloat MaterialRoughness = 0.01f;
            GLfloat MaterialMetallicity = 0.02f;
            GLfloat AmbientIntensity = 0.15f;
            /// UE4 dielectric fresnel
            vec3 MaterialF0 = vec3(0.04f);

            ///SAO
            GLint Turns = 7;
            GLint Samples = 12;
            GLint BlurSize = 4;
            GLint MotionBlurMaxSamples = 32;
            GLfloat Bias = 0.001f;
            GLfloat Scale = 0.7f;
            GLfloat Radius = 0.3f;
            GLfloat Contrast = 0.8f;

            /// IBL
            std::shared_ptr<Texture> SkyBox;

            /// Shaders
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

            std::shared_ptr<PointLight> Point;
            std::shared_ptr<DirectionalLight> Directional;

        private:
            GraphicsEngine& m_GraphicsEngine;

            int GetWindowWidth() const;
            int GetWindowHeight() const;

            void SAORendering(const mat4& proj);
            void LightRendering(const mat4& view, const mat4& proj);
            void PostProcessRendering(GLfloat deltaTime);
            void ForwardPassRendering(const mat4& view, const mat4& proj);

            bool m_PointMode = true;
            bool m_DirectionalMode = true;
            bool m_IBLMode = true;
            bool m_SAOMode = true;
            bool m_FXAAMode = true;
            bool m_MotionBlurMode = false;
            bool m_WireframeMode = false;

            ///IBL
            mat4 m_MapProjection;
            std::vector<mat4> m_MapView;
            std::shared_ptr<Texture> m_SkyBox;
            std::shared_ptr<Texture> m_MapCube;
            std::shared_ptr<Texture> m_MapIrradiance;
            std::shared_ptr<Texture> m_MapPrefilter;
            std::shared_ptr<Texture> m_MapLUT;
            std::shared_ptr<Cube> m_SceneCube;

            /// Pipeline settings
            GLint m_GBufferView = 1;
            GLint m_AttenuationMode = 1;
            GLint m_ToneMappingMode = 1;

            /// GBuffer
            GLuint m_GBufferFBO;
            GLuint m_DepthRBO;
            GLuint m_GPositionBuffer;
            GLuint m_GNormalBuffer;
            GLuint m_GAlbedoBuffer;
            GLuint m_GEffectsBuffer;

            ///SAO FBO
            GLuint m_SAOFBO;
            GLuint m_SAOBuffer;

            ///SAO Blur FBO
            GLuint m_SAOBlurFBO;
            GLuint m_SAOBlurBuffer;

            ///Post Process FBO
            GLuint m_PostProcessFBO;
            GLuint m_PostProcessBuffer;

            ///Cube FBO/RBO
            GLuint m_EnvToCubeFBO;
            GLuint m_EnvToCubeRBO;

            ///IBL FBO/RBO
            GLuint m_IrradianceFBO;
            GLuint m_IrradianceRBO;

            ///BRDF FBO/RBO
            GLuint m_BrdfLUTFBO;
            GLuint m_BrdfLUTRBO;

            ///Prefilter FBO/RBO
            GLuint m_PrefilterFBO;

            mat4 m_ProjViewModel = mat4(1.0f);
            mat4 m_ProjViewModelPrev = mat4(1.0f);

            // TODO - Use SetUniforms and UpdateUniforms to populate shaders
            // create vector of shaders that graphics engine will update in order
            // then delete these
            bool gBuffer = false;
            bool lightingBRDF = false;
            bool sao = false;
            bool firstPassProcess = false;
        };
    }
}

#pragma warning(push)