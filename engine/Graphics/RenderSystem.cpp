#include "stdafx.h"

#include "RenderSystem.h"
#include "../Core/Time/Timer.h"
#include "../Core/Math/MathFacade.h"
#include <iostream>

namespace
{
    const glm::vec3 CAMERA_INIT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
    const glm::vec3 CAMERA_INIT_FORWARD = glm::vec3(0.0f, 0.0f, -1.0f);
    const glm::vec3 CAMERA_INIT_UP = glm::vec3(0.0f, 1.0f, 0.0f);

    const float CAMERA_INIT_ASPECT = (16.0f / 9.0f);
    const float CAMERA_INIT_FOV = 54.0f;
    const float CAMERA_INIT_NEAR = 1.0f;    //THIS IS SET IN SHADER G-BUFFER.FRAG
    const float CAMERA_INIT_FAR = 1000.0f;  //THIS IS SET IN SHADER G-BUFFER.FRAG

    float CAMERA_INIT_APETURE = 16.0f;
    float CAMERA_INIT_SHUTTER_SPEED = 0.5f;
    float CAMERA_INIT_ISO = 1000.0f;
}

namespace Graphics
{
    namespace Render
    {
        RenderSystem::RenderSystem()
        {
            if (!glfwInit())
            {
                std::cerr << "Failed to init GLFW!" << std::endl;
                exit(1);
            }

            m_Window = new GLWindow("Caerulus", 800, 600, 32);

            glewExperimental = TRUE;

            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                std::cout << "OpenGL Error: " << error << std::endl;
            }

            if (glewInit() != GLEW_OK)
            {
                std::cerr << "Failed to init GLEW!" << std::endl;
                exit(1);
            }

            m_Camera = new Camera();
            m_Camera->SetFOV(CAMERA_INIT_FOV);
            m_Camera->SetAspect(CAMERA_INIT_ASPECT);
            m_Camera->SetNear(CAMERA_INIT_NEAR);
            m_Camera->SetFar(CAMERA_INIT_FAR);

            m_Camera->SetViewMatrix(glm::lookAt(
                CAMERA_INIT_POSITION,
                CAMERA_INIT_FORWARD + CAMERA_INIT_POSITION,
                CAMERA_INIT_UP));

            m_Camera->SetProjMatrix(glm::perspective(
                glm::radians(CAMERA_INIT_FOV),
                CAMERA_INIT_ASPECT,
                CAMERA_INIT_NEAR,
                CAMERA_INIT_FAR));

            m_Camera->SetAperture(CAMERA_INIT_APETURE);
            m_Camera->SetShutterSpeed(CAMERA_INIT_SHUTTER_SPEED);
            m_Camera->SetISO(CAMERA_INIT_ISO);

            m_PointMode         = true;
            m_DirectionalMode   = true;
            m_IBLMode           = true;
            m_SAOMode           = true;
            m_FXAAMode          = true;
            m_MotionBlurMode    = false;
            m_ProfilingMode     = false;
            m_WireframeMode     = false;

            m_GBufferView       = 1;
            m_ToneMappingMode   = 1;
            m_AttenuationMode   = 2;

            m_MaterialRoughness     = 0.01f;
            m_MaterialMetallicity   = 0.02f;
            m_AmbientIntensity      = 0.15f;

            m_MaterialF0        = vec3(0.04f);

            m_ProjViewModel     = mat4(1.0f);
            m_ProjViewModelPrev = m_ProjViewModel;

            mat4 quadTransform;
            Core::Math::CreateTansform(quadTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
            m_WindowQuad        = new QuadGeometry(quadTransform);

            // Temp lights
            m_PointLightMap.insert(std::make_pair(0, new PointLight(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(0.0, -1.0f, 0.0f), 5.0)));
            m_DirectionalLightMap.insert(std::make_pair(0, new DirectionalLight(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(0.0, -1.0f, 0.0f))));


            //GPU Profiling
            m_Profiler = new GPUProfiler();
            m_Profiler->DeltaGeometryTime    = 0.0f;
            m_Profiler->DeltaLightingTime    = 0.0f;
            m_Profiler->DeltaSAOTime         = 0.0f;
            m_Profiler->DeltaPostProcessTime = 0.0f;
            m_Profiler->DeltaForwardTime     = 0.0f;

            //SAO
            m_SAO = new SAO();
            m_SAO->Samples               = 12;
            m_SAO->Turns                 = 7;
            m_SAO->BlurSize              = 4;
            m_SAO->Radius                = 0.3f;
            m_SAO->Bias                  = 0.001f;
            m_SAO->Scale                 = 0.7f;
            m_SAO->Contrast              = 0.8f;
            m_SAO->MotionBlurMaxSamples  = 32;

            //IBL
            m_IBL = new IBL();
            m_IBL->MapLUT.CreateHDR(512, 512, GL_RG, GL_RG16F, GL_FLOAT, GL_LINEAR);
            m_IBL->MapCube.CreateCube(512, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_IBL->MapIrradiance.CreateCube(32, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR);
            m_IBL->MapPrefilter.CreateCube(128, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_IBL->MapPrefilter.ComputeMipmap();

            mat4 sceneCubeTransform;
            Core::Math::CreateTansform(sceneCubeTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
            m_IBL->SceneCube = CubeGeometry(sceneCubeTransform);

            m_IBL->MapProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),  vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),  vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));

            SetCapabilities();
            LoadShaders();
            SetShaderUniformLocations();
            GBufferSetup();
            SAOBufferSetup();
            PostProcessBufferSetup();
            InitProfileQuery();
        }

        bool RenderSystem::InitGLEW() const
        {
            // Use glewExperimental so GLEW forces the use of modern OpengGL functions.
            glewExperimental = GL_TRUE;
            if (glewInit() != GLEW_OK)
            {
                std::cerr << "Error initialising GLEW." << std::endl;
                return false;
            }
            return true;
        }

        void RenderSystem::SetCapabilities() const
        {
            glViewport(0, 0, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LESS);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void RenderSystem::LoadShaders()
        {
            m_Shaders = new StandardShaders();

            m_Shaders->GBuffer.Load(
                "assets/shaders/gBuffer.vert",
                "assets/shaders/gBuffer.frag");

            m_Shaders->LatlongToCube.Load(
                "assets/shaders/latlongToCube.vert",
                "assets/shaders/latlongToCube.frag");

            m_Shaders->Simple.Load(
                "assets/shaders/lighting/simple.vert",
                "assets/shaders/lighting/simple.frag");

            m_Shaders->LightingBRDF.Load(
                "assets/shaders/lighting/lightingBRDF.vert",
                "assets/shaders/lighting/lightingBRDF.frag");

            m_Shaders->IrradianceIBL.Load(
                "assets/shaders/lighting/irradianceIBL.vert",
                "assets/shaders/lighting/irradianceIBL.frag");

            m_Shaders->PrefilterIBL.Load(
                "assets/shaders/lighting/prefilterIBL.vert",
                "assets/shaders/lighting/prefilterIBL.frag");

            m_Shaders->IntegrateIBL.Load(
                "assets/shaders/lighting/integrateIBL.vert",
                "assets/shaders/lighting/integrateIBL.frag");

            m_Shaders->FirstPassPostProcess.Load(
                "assets/shaders/postprocess/postprocess.vert",
                "assets/shaders/postprocess/firstpass.frag");

            m_Shaders->SAO.Load(
                "assets/shaders/postprocess/sao.vert",
                "assets/shaders/postprocess/sao.frag");

            m_Shaders->SAOBlur.Load(
                "assets/shaders/postprocess/sao.vert",
                "assets/shaders/postprocess/saoBlur.frag");
        }

        void RenderSystem::SetShaderUniformLocations()
        {
            m_Shaders->GBuffer.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "texAlbedo"), 0);
            glUniform1i(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "texNormal"), 1);
            glUniform1i(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "texRoughness"), 2);
            glUniform1i(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "texMetalness"), 3);
            glUniform1i(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "texAO"), 4);

            m_Shaders->LightingBRDF.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "gPosition"), 0);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "gAlbedo"), 1);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "gNormal"), 2);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "gEffects"), 3);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "sao"), 4);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "envMap"), 5);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "envMapIrradiance"), 6);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "envMapPrefilter"), 7);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "envMapLUT"), 8);

            m_Shaders->SAO.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "gPosition"), 0);
            glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "gNormal"), 1);

            m_Shaders->FirstPassPostProcess.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "sao"), 1);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "gEffects"), 2);

            m_Shaders->LatlongToCube.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->LatlongToCube.GetHandle(), "envMap"), 0);

            m_Shaders->IrradianceIBL.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->IrradianceIBL.GetHandle(), "envMap"), 0);

            m_Shaders->PrefilterIBL.Use();
            glUniform1i(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "envMap"), 0);
        }

        void RenderSystem::GBufferSetup()
        {
            glGenFramebuffers(1, &m_GBufferFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);

            // Position
            glGenTextures(1, &m_GPositionBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPositionBuffer, 0);

            // Albedo + Roughness
            glGenTextures(1, &m_GAlbedoBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GAlbedoBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GAlbedoBuffer, 0);

            // Normals + Metalness
            glGenTextures(1, &m_GNormalBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GNormalBuffer, 0);

            // Effects (AO + Velocity)
            glGenTextures(1, &m_GEffectsBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GEffectsBuffer, 0);

            // Define the COLOR_ATTACHMENTS for the G-Buffer
            GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(4, attachments);

            // Z-Buffer
            glGenRenderbuffers(1, &m_DepthRBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthRBO);

            // Check if the framebuffer is complete before continuing
            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "Framebuffer not complete !" << std::endl;
            }
        }

        void RenderSystem::SAOBufferSetup()
        {
            // SAO Buffer
            glGenFramebuffers(1, &m_SAOFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_SAOFBO);
            glGenTextures(1, &m_SAOBuffer);
            glBindTexture(GL_TEXTURE_2D, m_SAOBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SAOBuffer, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "SAO Framebuffer not complete !" << std::endl;
            }

            // SAO Blur Buffer
            glGenFramebuffers(1, &m_SAOBlurFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_SAOBlurFBO);
            glGenTextures(1, &m_SAOBlurBuffer);
            glBindTexture(GL_TEXTURE_2D, m_SAOBlurBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_SAOBlurBuffer, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "SAO Blur Framebuffer not complete !" << std::endl;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void RenderSystem::PostProcessBufferSetup()
        {
            // Post-processing Buffer
            glGenFramebuffers(1, &m_PostProcessFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);

            glGenTextures(1, &m_PostProcessBuffer);
            glBindTexture(GL_TEXTURE_2D, m_PostProcessBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height, 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PostProcessBuffer, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            {
                std::cerr << "Postprocess Framebuffer not complete !" << std::endl;
            }
        }

        void RenderSystem::IBLBufferSetup()
        {
            // Latlong to Cubemap conversion
            glGenFramebuffers(1, &m_EnvToCubeFBO);
            glGenRenderbuffers(1, &m_EnvToCubeRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_EnvToCubeFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_EnvToCubeRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapCube.GetWidth(), m_IBL->MapCube.GetHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_EnvToCubeRBO);

            m_Shaders->LatlongToCube.Use();

            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LatlongToCube.GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            glActiveTexture(GL_TEXTURE0);
            m_SkyBox->UseTexture();

            glViewport(0, 0, m_IBL->MapCube.GetWidth(), m_IBL->MapCube.GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_EnvToCubeFBO);

            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LatlongToCube.GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapCube.GetID(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
               
                m_IBL->SceneCube.Draw();
            }

            m_IBL->MapCube.ComputeMipmap();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Diffuse irradiance capture
            glGenFramebuffers(1, &m_IrradianceFBO);
            glGenRenderbuffers(1, &m_IrradianceRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapIrradiance.GetWidth(), m_IBL->MapIrradiance.GetHeight());

            m_Shaders->IrradianceIBL.Use();

            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->IrradianceIBL.GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            glActiveTexture(GL_TEXTURE0);
            m_IBL->MapCube.UseTexture();

            glViewport(0, 0, m_IBL->MapIrradiance.GetWidth(), m_IBL->MapIrradiance.GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);

            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->IrradianceIBL.GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapIrradiance.GetID(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_IBL->SceneCube.Draw();
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Prefilter cubemap
            m_Shaders->PrefilterIBL.Use();

            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            m_IBL->MapCube.UseTexture();

            glGenFramebuffers(1, &m_PrefilterFBO);
            glGenRenderbuffers(1, &m_PrefilterRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_PrefilterFBO);

            unsigned int maxMipLevels = 5;

            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                GLsizei mipWidth = static_cast<GLsizei>(m_IBL->MapPrefilter.GetWidth() * std::pow(0.5, mip));
                GLsizei mipHeight = static_cast<GLsizei>(m_IBL->MapPrefilter.GetHeight() * std::pow(0.5, mip));

                glBindRenderbuffer(GL_RENDERBUFFER, m_PrefilterRBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);

                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);

                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "roughness"), roughness);
                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "cubeResolutionWidth"), static_cast<GLfloat>(m_IBL->MapPrefilter.GetWidth()));
                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "cubeResolutionHeight"), static_cast<GLfloat>(m_IBL->MapPrefilter.GetHeight()));

                for (unsigned int i = 0; i < 6; ++i)
                {
                    glUniformMatrix4fv(glGetUniformLocation(m_Shaders->PrefilterIBL.GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapPrefilter.GetID(), mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_IBL->SceneCube.Draw();
                }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // BRDF LUT
            glGenFramebuffers(1, &m_BrdfLUTFBO);
            glGenRenderbuffers(1, &m_BrdfLUTRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_BrdfLUTFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_BrdfLUTRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapLUT.GetWidth(), m_IBL->MapLUT.GetHeight());
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IBL->MapLUT.GetID(), 0);

            glViewport(0, 0, m_IBL->MapLUT.GetWidth(), m_IBL->MapLUT.GetHeight());
            m_Shaders->IntegrateIBL.Use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_WindowQuad->Draw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glViewport(0, 0, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height);
        }

        void RenderSystem::InitProfileQuery()
        {
            glGenQueries(2, m_Profiler->QueryIDGeometry);
            glGenQueries(2, m_Profiler->QueryIDLighting);
            glGenQueries(2, m_Profiler->QueryIDSAO);
            glGenQueries(2, m_Profiler->QueryIDPostprocess);
            glGenQueries(2, m_Profiler->QueryIDForward);
        }

        void RenderSystem::DrawAll()
        {
            //Time taken to render geometry
            glQueryCounter(m_Profiler->QueryIDGeometry[GPUProfiler::Start], GL_TIMESTAMP);

            mat4 view = m_Camera->GetViewMatrix() /** this by transform of camera*/;
            mat4 proj = m_Camera->GetProjMatrix();

            // geo rendering
            m_Shaders->GBuffer.Use();
            SetCameraUniforms(view, proj, m_Camera->GetNear(), m_Camera->GetFar());

            for (auto model : m_ModelMap)
            {
                if (model.second)
                {
                    //Todo add material rendering
                    mat4 transform = *m_TransformMap.at(model.first);

                    m_ProjViewModel = proj * view * transform;
                    SetModelUniforms(transform);
                    model.second->Draw(m_WireframeMode);
                }
            }

            glQueryCounter(m_Profiler->QueryIDGeometry[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void RenderSystem::SetCameraUniforms(const mat4& view, const mat4& proj, float nearPlane, float farPlane) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "nearPlane"), 1, GL_FALSE, &nearPlane);
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "farPlane"), 1, GL_FALSE, &farPlane);
        }

        void RenderSystem::SetModelUniforms(const mat4& model) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "projViewModel"), 1, GL_FALSE, value_ptr(m_ProjViewModel));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "projViewModelPrev"), 1, GL_FALSE, value_ptr(m_ProjViewModelPrev));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer.GetHandle(), "model"), 1, GL_FALSE, value_ptr(model));
        }

        void RenderSystem::Update(float deltaTime)
        {
            FlushErrors();
            Clear();
            DrawAll();
            SwapBuffer(deltaTime);
        }

        void RenderSystem::SetCamera(Camera& camera)
        {
            *m_Camera = camera;
        }

        Camera& RenderSystem::GetCamera() const
        {
            return *m_Camera;
        }

        void RenderSystem::Clear() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void RenderSystem::FlushErrors() const
        {
            while (glGetError() != GL_NO_ERROR)
            {
            }
        }

        void RenderSystem::SwapBuffer(float frameRate)
        {
            mat4 view = m_Camera->GetViewMatrix()/* * m_TransformCM->GetTransform(camera)*/;
            mat4 proj = m_Camera->GetProjMatrix();

            //update previous projviewmodel
            m_ProjViewModelPrev = m_ProjViewModel;

            SAORendering(proj);
            LightRendering(view, proj);
            PostProcessRendering(frameRate, m_Camera->GetAperture(), m_Camera->GetShutterSpeed(), m_Camera->GetISO());
            ForwardPassRendering(view, proj);
            if (m_ProfilingMode)
            {
                GPUProfiling();
            }

            m_Window->SwapBuffer();
        }

        void RenderSystem::GPUProfiling()
        {
            GLuint64 startGeometryTime, startLightingTime, startSAOTime, startPostProcessTime, startForwardTime;
            GLuint64 stopGeometryTime, stopLightingTime, stopSAOTime, stopPostProcessTime, stopForwardTime;

            GLint stopGeometryTimerAvailable = 0;
            GLint stopLightingTimerAvailable = 0;
            GLint stopSAOTimerAvailable = 0;
            GLint stopPostprocessTimerAvailable = 0;
            GLint stopForwardTimerAvailable = 0;

            while (!stopGeometryTimerAvailable
                && !stopLightingTimerAvailable
                && !stopSAOTimerAvailable
                && !stopPostprocessTimerAvailable
                && !stopForwardTimerAvailable)
            {
                glGetQueryObjectiv(m_Profiler->QueryIDGeometry[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopGeometryTimerAvailable);
                glGetQueryObjectiv(m_Profiler->QueryIDLighting[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopLightingTimerAvailable);
                glGetQueryObjectiv(m_Profiler->QueryIDSAO[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopSAOTimerAvailable);
                glGetQueryObjectiv(m_Profiler->QueryIDPostprocess[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopPostprocessTimerAvailable);
                glGetQueryObjectiv(m_Profiler->QueryIDForward[GPUProfiler::Stop], GL_QUERY_RESULT_AVAILABLE, &stopForwardTimerAvailable);
            }

            glGetQueryObjectui64v(m_Profiler->QueryIDGeometry[GPUProfiler::Start], GL_QUERY_RESULT, &startGeometryTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDGeometry[GPUProfiler::Stop], GL_QUERY_RESULT, &stopGeometryTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDLighting[GPUProfiler::Start], GL_QUERY_RESULT, &startLightingTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDLighting[GPUProfiler::Stop], GL_QUERY_RESULT, &stopLightingTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDSAO[GPUProfiler::Start], GL_QUERY_RESULT, &startSAOTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDSAO[GPUProfiler::Stop], GL_QUERY_RESULT, &stopSAOTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDPostprocess[GPUProfiler::Start], GL_QUERY_RESULT, &startPostProcessTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDPostprocess[GPUProfiler::Stop], GL_QUERY_RESULT, &stopPostProcessTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDForward[GPUProfiler::Start], GL_QUERY_RESULT, &startForwardTime);
            glGetQueryObjectui64v(m_Profiler->QueryIDForward[GPUProfiler::Stop], GL_QUERY_RESULT, &stopForwardTime);

            m_Profiler->DeltaGeometryTime    = (float)((stopGeometryTime       - startGeometryTime)    * Core::Time::MILLISECOND);
            m_Profiler->DeltaLightingTime    = (float)((stopLightingTime       - startLightingTime)    * Core::Time::MILLISECOND);
            m_Profiler->DeltaSAOTime         = (float)((stopSAOTime            - startSAOTime)         * Core::Time::MILLISECOND);
            m_Profiler->DeltaPostProcessTime = (float)((stopPostProcessTime    - startPostProcessTime) * Core::Time::MILLISECOND);
            m_Profiler->DeltaForwardTime     = (float)((stopForwardTime        - startForwardTime)     * Core::Time::MILLISECOND);
        }

        void RenderSystem::SAORendering(const mat4& proj)
        {
            glQueryCounter(m_Profiler->QueryIDSAO[GPUProfiler::Start], GL_TIMESTAMP);
            glBindFramebuffer(GL_FRAMEBUFFER, m_SAOFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            if (m_SAOMode)
            {
                // SAO noisy texture
                m_Shaders->SAO.Use();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);

                glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoSamples"), m_SAO->Samples);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoRadius"), m_SAO->Radius);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoTurns"), m_SAO->Turns);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoBias"), m_SAO->Bias);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoScale"), m_SAO->Scale);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "saoContrast"), m_SAO->Contrast);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "viewportWidth"), m_Window->GetActiveState().Width);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO.GetHandle(), "viewportHeight"), m_Window->GetActiveState().Height);

                m_WindowQuad->Draw();

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // SAO blur pass
                glBindFramebuffer(GL_FRAMEBUFFER, m_SAOBlurFBO);
                glClear(GL_COLOR_BUFFER_BIT);

                m_Shaders->SAOBlur.Use();

                glUniform1i(glGetUniformLocation(m_Shaders->SAOBlur.GetHandle(), "saoBlurSize"), m_SAO->BlurSize);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_SAOBuffer);

                m_WindowQuad->Draw();

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            glQueryCounter(m_Profiler->QueryIDSAO[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void RenderSystem::LightRendering(const mat4& view, const mat4& proj)
        {
            glQueryCounter(m_Profiler->QueryIDLighting[GPUProfiler::Start], GL_TIMESTAMP);

            glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Shaders->LightingBRDF.Use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_GAlbedoBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, m_SAOBlurBuffer);
            glActiveTexture(GL_TEXTURE5);
            if (m_SkyBox)
            {
                m_SkyBox->UseTexture();
                glActiveTexture(GL_TEXTURE6);
                m_IBL->MapIrradiance.UseTexture();
                glActiveTexture(GL_TEXTURE7);
                m_IBL->MapPrefilter.UseTexture();
                glActiveTexture(GL_TEXTURE8);
                m_IBL->MapLUT.UseTexture();
            }

            for (auto idPointPair : m_PointLightMap)
            {
                if (idPointPair.second)
                {
                    idPointPair.second->RenderToShader(idPointPair.first, m_Shaders->LightingBRDF, view);
                }
            }

            for (auto idDirectionalPair : m_DirectionalLightMap)
            {
                if (idDirectionalPair.second)
                {
                    idDirectionalPair.second->RenderToShader(idDirectionalPair.first, m_Shaders->LightingBRDF, view);
                }
            }

            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "transposedView"), 1, GL_FALSE, value_ptr(transpose(view)));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "inverseProj"), 1, GL_FALSE, value_ptr(inverse(proj)));
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "materialRoughness"), m_MaterialRoughness);
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "materialMetallicity"), m_MaterialMetallicity);
            glUniform3f(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "materialF0"), m_MaterialF0.r, m_MaterialF0.g, m_MaterialF0.b);
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "ambientIntensity"), m_AmbientIntensity);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "gBufferView"), m_GBufferView);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "pointMode"), m_PointMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "directionalMode"), m_DirectionalMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "iblMode"), m_IBLMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF.GetHandle(), "attenuationMode"), m_AttenuationMode);

            m_WindowQuad->Draw();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glQueryCounter(m_Profiler->QueryIDLighting[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void RenderSystem::PostProcessRendering(GLfloat frameRate, GLfloat cameraAperture, GLfloat cameraShutterSpeed, GLfloat cameraISO)
        {
            glQueryCounter(m_Profiler->QueryIDPostprocess[GPUProfiler::Start], GL_TIMESTAMP);

            glClear(GL_COLOR_BUFFER_BIT);
            m_Shaders->FirstPassPostProcess.Use();

            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "gBufferView"), m_GBufferView);
            glUniform2f(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "screenTextureSize"), 1.0f / m_Window->GetActiveState().Width, 1.0f / m_Window->GetActiveState().Height);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "cameraAperture"), cameraAperture);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "cameraShutterSpeed"), cameraShutterSpeed);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "cameraISO"), cameraISO);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "saoMode"), m_SAOMode);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "fxaaMode"), m_FXAAMode);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "motionBlurMode"), m_MotionBlurMode);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "motionBlurScale"), frameRate);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "motionBlurMaxSamples"), m_SAO->MotionBlurMaxSamples);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess.GetHandle(), "tonemappingMode"), m_ToneMappingMode);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_PostProcessBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_SAOBlurBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);

            m_WindowQuad->Draw();

            glQueryCounter(m_Profiler->QueryIDPostprocess[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void RenderSystem::ForwardPassRendering(const mat4& view, const mat4& proj)
        {
            glQueryCounter(m_Profiler->QueryIDForward[GPUProfiler::Start], GL_TIMESTAMP);

            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBufferFBO);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // Copy the depth informations from the Geometry Pass into the default framebuffer
            glBlitFramebuffer(
                0, 0,
                m_Window->GetActiveState().Width,
                m_Window->GetActiveState().Height,
                0, 0,
                m_Window->GetActiveState().Width,
                m_Window->GetActiveState().Height,
                GL_DEPTH_BUFFER_BIT, GL_NEAREST);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            if (m_PointMode)
            {
                m_Shaders->Simple.Use();
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->Simple.GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->Simple.GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));

                for (auto idPointPair : m_PointLightMap)
                {
                    if (idPointPair.second)
                    {
                        auto colour = idPointPair.second->GetColour();
                        glUniform4f(glGetUniformLocation(m_Shaders->Simple.GetHandle(), "lightColor"), colour.r, colour.g, colour.b, colour.a);
                    }
                }

            }
            glQueryCounter(m_Profiler->QueryIDForward[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void RenderSystem::SetNullMaterial()
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE4);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void RenderSystem::ToggleWireframe()
        {
            m_WireframeMode = !m_WireframeMode;
        }

        void RenderSystem::ToggleSAO()
        {
            m_SAOMode = !m_SAOMode;
        }

        void RenderSystem::ToggleFXAA()
        {
            m_FXAAMode = !m_FXAAMode;
        }

        void RenderSystem::ToggleMotionBlur()
        {
            m_MotionBlurMode = !m_MotionBlurMode;
        }

        void RenderSystem::TogglePointLightRender()
        {
            m_PointMode = !m_PointMode;
        }

        void RenderSystem::ToggleDirectionalLightRender()
        {
            m_DirectionalMode = !m_DirectionalMode;
        }

        void RenderSystem::ToggleEnviromentLightRender()
        {
            m_IBLMode = !m_IBLMode;
        }

        void RenderSystem::ToggleToneMapping(unsigned int tone)
        {
            m_ToneMappingMode = tone;
        }

        IBL& RenderSystem::GetIBL() const
        {
            return *m_IBL;
        }

        SAO& RenderSystem::GetSAO() const
        {
            return *m_SAO;
        }

        GPUProfiler& RenderSystem::GetRenderProfiler() const
        {
            return *m_Profiler;
        }

        StandardShaders& RenderSystem::GetShaders() const
        {
            return *m_Shaders;
        }

        void RenderSystem::SetSkyBox(Texture& skyBox)
        {
            m_SkyBox = &skyBox;
            IBLBufferSetup();
        }

        Texture& RenderSystem::GetSkyBox() const
        {
            return *m_SkyBox;
        }

        void RenderSystem::SetDefaultAO(Texture& ao)
        {
            m_DefualtAO = &ao;
        }

        Texture& RenderSystem::GetDefaultAO() const
        {
            return *m_DefualtAO;
        }

        void RenderSystem::SetDefaultMaterial(Material& material)
        {
            m_DefaultMaterial = &material;
        }

        Material& RenderSystem::GetDefaultMaterial() const
        {
            return *m_DefaultMaterial;
        }

        void RenderSystem::SetPointLightMap(std::map<unsigned int, Graphics::Light::PointLight*> idPointMap)
        {
            m_PointLightMap = idPointMap;
        }

        const std::map<unsigned int, Graphics::Light::PointLight*>& RenderSystem::GetPointLightMap() const
        {
            return m_PointLightMap;
        }

        void RenderSystem::SetDirectionalLightMap(std::map<unsigned int, Graphics::Light::DirectionalLight*> idDirectionalMap)
        {
            m_DirectionalLightMap = idDirectionalMap;
        }

        const std::map<unsigned int, Graphics::Light::DirectionalLight*>& RenderSystem::GetDirectionalLightMap() const
        {
            return m_DirectionalLightMap;
        }

        void RenderSystem::SetGLWindow(GLWindow& window)
        {
            m_Window = &window;
        }

        GLWindow& RenderSystem::GetGLWindow() const
        {
            return *m_Window;
        }

        void RenderSystem::SetModelMap(const std::map<unsigned int, Model*>& modelMap)
        {
            m_ModelMap = modelMap;
        }

        const std::map<unsigned int, Model*>& RenderSystem::GetModelMap() const
        {
            return m_ModelMap;
        }

        void RenderSystem::SetTransformMap(const std::map<unsigned int, mat4*>& transformMap)
        {
            m_TransformMap = transformMap;
        }

        const std::map<unsigned int, mat4*>& RenderSystem::GetTransformMap() const
        {
            return m_TransformMap;
        }
    }
}