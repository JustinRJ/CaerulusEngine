#include "stdafx.h"

#include "GraphicsEngine.h"
#include "../Core/Time/Timer.h"
#include "../Core/Math/MathHelper.h"
#include "../Core/Logging/Log.h"

namespace Graphics
{
    namespace Render
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
            MathHelper::CreateTansform(quadTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
            m_Window->SetQuad(std::make_shared<Quad>(quadTransform));

            // Temp lights
            // lightingBRDF.frag only supports 1 light of each type - easily set
            m_PointLightMap.insert(std::make_pair(0, std::make_shared<PointLight>(vec4(1.0f, 1.0f, 1.0f, 0.0f), vec3(0.0, 25.0f, 0.0f), 10000.0f)));
            m_DirectionalLightMap.insert(std::make_pair(0, std::make_shared<DirectionalLight>(vec4(1.0f, 1.0f, 1.0f, 1.0f), vec3(0.0, -1.0f, 0.0f))));

            //SAO
            m_SAO = std::make_shared<SAO>();

            //IBL
            m_IBL = std::make_shared<IBL>();
            m_IBL->MapLUT.CreateHDR(512, 512, GL_RG, GL_RG16F, GL_FLOAT, GL_LINEAR);
            m_IBL->MapCube.CreateCube(512, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_IBL->MapIrradiance.CreateCube(32, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR);
            m_IBL->MapPrefilter.CreateCube(128, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_IBL->MapPrefilter.ComputeMipmap();

            mat4 sceneCubeTransform;
            MathHelper::CreateTansform(sceneCubeTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
            m_IBL->SceneCube = Cube(sceneCubeTransform);

            m_IBL->MapProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),  vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f),  vec3(0.0f, 0.0f, 1.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f),  vec3(0.0f, -1.0f, 0.0f)));
            m_IBL->MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));

            m_Profiler.InitProfileQuery();

            glViewport(0, 0, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glDepthFunc(GL_LESS);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void GraphicsEngine::LoadShaders()
        {
            /// GBUFFER
            /// ------------------------------------------------------------------------------------------------------------------------------
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
                using Core::Logging::Log;
                Log::LogError("Framebuffer not complete!");
            }


            /// ------------------------------------------------------------------------------------------------------------------------------
            /// SAO Buffer
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
                using Core::Logging::Log;
                Log::LogError("SAO Framebuffer not complete!");
            }


            /// SAO Blur Buffer
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
                using Core::Logging::Log;
                Log::LogError("SAO Blur Framebuffer not complete!");
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// ------------------------------------------------------------------------------------------------------------------------------
            /// Post-processing Buffer
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
                using Core::Logging::Log;
                Log::LogError("Postprocess Framebuffer not complete!");
            }


            /// ------------------------------------------------------------------------------------------------------------------------------
            /// Latlong to Cubemap conversion
            glGenFramebuffers(1, &m_EnvToCubeFBO);
            glGenRenderbuffers(1, &m_EnvToCubeRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_EnvToCubeFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_EnvToCubeRBO);


            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapCube.GetWidth(), m_IBL->MapCube.GetHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_EnvToCubeRBO);
            m_Shaders->LatlongToCube->Bind();
            glUniform1i(glGetUniformLocation(m_Shaders->LatlongToCube->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LatlongToCube->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            glActiveTexture(GL_TEXTURE0);
            m_SkyBox->Bind();
            glViewport(0, 0, m_IBL->MapCube.GetWidth(), m_IBL->MapCube.GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_EnvToCubeFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LatlongToCube->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapCube.GetHandle(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_IBL->SceneCube.Draw(false /*wireframe*/);
            }
            m_IBL->MapCube.ComputeMipmap();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// Diffuse irradiance capture
            glGenFramebuffers(1, &m_IrradianceFBO);
            glGenRenderbuffers(1, &m_IrradianceRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRBO);


            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapIrradiance.GetWidth(), m_IBL->MapIrradiance.GetHeight());
            m_Shaders->IrradianceIBL->Bind();
            glUniform1i(glGetUniformLocation(m_Shaders->IrradianceIBL->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->IrradianceIBL->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            glActiveTexture(GL_TEXTURE0);
            m_IBL->MapCube.Bind();
            glViewport(0, 0, m_IBL->MapIrradiance.GetWidth(), m_IBL->MapIrradiance.GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->IrradianceIBL->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapIrradiance.GetHandle(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_IBL->SceneCube.Draw(false /*wireframe*/);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// Prefilter cubemap
            m_Shaders->PrefilterIBL->Bind();
            glUniform1i(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_IBL->MapProjection));
            m_IBL->MapCube.Bind();


            glGenFramebuffers(1, &m_PrefilterFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_PrefilterFBO);


            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                GLsizei mipWidth = static_cast<GLsizei>(m_IBL->MapPrefilter.GetWidth() * std::pow(0.5, mip));
                GLsizei mipHeight = static_cast<GLsizei>(m_IBL->MapPrefilter.GetHeight() * std::pow(0.5, mip));

                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);

                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "roughness"), roughness);
                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "cubeResolutionWidth"), static_cast<GLfloat>(m_IBL->MapPrefilter.GetWidth()));
                glUniform1f(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "cubeResolutionHeight"), static_cast<GLfloat>(m_IBL->MapPrefilter.GetHeight()));

                for (unsigned int i = 0; i < 6; ++i)
                {
                    glUniformMatrix4fv(glGetUniformLocation(m_Shaders->PrefilterIBL->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_IBL->MapView[i]));
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IBL->MapPrefilter.GetHandle(), mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_IBL->SceneCube.Draw(false /*wireframe*/);
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// BRDF LUT
            glGenFramebuffers(1, &m_BrdfLUTFBO);
            glGenRenderbuffers(1, &m_BrdfLUTRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_BrdfLUTFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_BrdfLUTRBO);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_IBL->MapLUT.GetWidth(), m_IBL->MapLUT.GetHeight());
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IBL->MapLUT.GetHandle(), 0);
            glViewport(0, 0, m_IBL->MapLUT.GetWidth(), m_IBL->MapLUT.GetHeight());
            m_Shaders->IntegrateIBL->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);






            m_Window->GetQuad()->Draw(false /*wireframe*/);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, m_Window->GetActiveState().Width, m_Window->GetActiveState().Height);
        }



        void GraphicsEngine::DrawAll()
        {
            //Time taken to render geometry
            glQueryCounter(m_Profiler.QueryIDGeometry[GPUProfiler::Start], GL_TIMESTAMP);

            // geo rendering
            if (m_Shaders->GBuffer)
            {
                m_Shaders->GBuffer->Bind();

                if (!gBuffer)
                {
                    glUniform1i(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "texAlbedo"), 0);
                    glUniform1i(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "texNormal"), 1);
                    glUniform1i(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "texRoughness"), 2);
                    glUniform1i(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "texMetalness"), 3);
                    glUniform1i(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "texAO"), 4);
                    gBuffer = true;
                }

                float nearPlane = m_Camera->GetNear();
                float farPlane = m_Camera->GetFar();
                mat4 view = m_Camera->GetViewMatrix() /** this by transform of camera*/;
                mat4 proj = m_Camera->GetProjMatrix();
                mat4 cameraTransform;
                m_Camera->GetProjViewModel(cameraTransform);
                m_ProjViewModel = cameraTransform;
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "nearPlane"), 1, GL_FALSE, &nearPlane);
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "farPlane"), 1, GL_FALSE, &farPlane);
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "projViewModel"), 1, GL_FALSE, value_ptr(m_ProjViewModel));
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "projViewModelPrev"), 1, GL_FALSE, value_ptr(m_ProjViewModelPrev));

                for (auto model : m_ModelMap)
                {
                    if (model.second)
                    {
                        const mat4& transform = *m_TransformMap.at(model.first);
                        glUniformMatrix4fv(glGetUniformLocation(m_Shaders->GBuffer->GetHandle(), "model"), 1, GL_FALSE, value_ptr(transform));
                        model.second->Draw(m_WireframeMode, m_DefaultMaterial);
                    }
                }
            }

            glQueryCounter(m_Profiler.QueryIDGeometry[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void GraphicsEngine::PreUpdate(float deltaTime)
        {
            FlushErrors();
            Clear();
        }


        void GraphicsEngine::Update(float deltaTime)
        {
            m_Window->Update();
            DrawAll();
            SwapBuffer(deltaTime);
        }

        void GraphicsEngine::SetCamera(std::shared_ptr<Camera> camera)
        {
            m_Camera = camera;
        }

        std::shared_ptr<Camera> GraphicsEngine::GetCamera()
        {
            return m_Camera;
        }

        void GraphicsEngine::Clear() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void GraphicsEngine::FlushErrors() const
        {
            while (glGetError() != GL_NO_ERROR)
            {
            }
        }

        void GraphicsEngine::SwapBuffer(float deltaTime)
        {
            mat4 view = m_Camera->GetViewMatrix()/* * m_TransformCM->GetTransform(camera)*/;
            mat4 proj = m_Camera->GetProjMatrix();

            //update previous projviewmodel
            m_ProjViewModelPrev = m_ProjViewModel;

            SAORendering(proj);
            LightRendering(view, proj);
            PostProcessRendering(deltaTime);
            ForwardPassRendering(view, proj);
            if (m_ProfilingMode)
            {
                ProfileGPUs();
            }

            m_Window->SwapBuffer();
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

        void GraphicsEngine::SAORendering(const mat4& proj)
        {
            glQueryCounter(m_Profiler.QueryIDSAO[GPUProfiler::Start], GL_TIMESTAMP);
            glBindFramebuffer(GL_FRAMEBUFFER, m_SAOFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            if (m_Shaders->SAO && m_SAOMode)
            {
                // SAO noisy texture
                m_Shaders->SAO->Bind();

                if (!sao)
                {
                    glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "gPosition"), 0);
                    glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "gNormal"), 1);
                }

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);

                glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoSamples"), m_SAO->Samples);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoRadius"), m_SAO->Radius);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoTurns"), m_SAO->Turns);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoBias"), m_SAO->Bias);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoScale"), m_SAO->Scale);
                glUniform1f(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "saoContrast"), m_SAO->Contrast);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "viewportWidth"), m_Window->GetActiveState().Width);
                glUniform1i(glGetUniformLocation(m_Shaders->SAO->GetHandle(), "viewportHeight"), m_Window->GetActiveState().Height);

                m_Window->GetQuad()->Draw(false /*wireframe*/);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // SAO blur pass
                glBindFramebuffer(GL_FRAMEBUFFER, m_SAOBlurFBO);
                glClear(GL_COLOR_BUFFER_BIT);

                m_Shaders->SAOBlur->Bind();

                glUniform1i(glGetUniformLocation(m_Shaders->SAOBlur->GetHandle(), "saoBlurSize"), m_SAO->BlurSize);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_SAOBuffer);

                m_Window->GetQuad()->Draw(false /*wireframe*/);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
            glQueryCounter(m_Profiler.QueryIDSAO[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void GraphicsEngine::LightRendering(const mat4& view, const mat4& proj)
        {
            glQueryCounter(m_Profiler.QueryIDLighting[GPUProfiler::Start], GL_TIMESTAMP);

            glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_Shaders->LightingBRDF->Bind();

            if (!lightingBRDF)
            {
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "gPosition"), 0);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "gAlbedo"), 1);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "gNormal"), 2);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "gEffects"), 3);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "sao"), 4);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "envMap"), 5);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "envMapIrradiance"), 6);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "envMapPrefilter"), 7);
                glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "envMapLUT"), 8);
                lightingBRDF = true;
            }

            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "transposedView"), 1, GL_FALSE, value_ptr(transpose(view)));
            glUniformMatrix4fv(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "inverseProj"), 1, GL_FALSE, value_ptr(inverse(proj)));
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "materialRoughness"), m_MaterialRoughness);
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "materialMetallicity"), m_MaterialMetallicity);
            glUniform3f(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "materialF0"), m_MaterialF0.r, m_MaterialF0.g, m_MaterialF0.b);
            glUniform1f(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "ambientIntensity"), m_AmbientIntensity);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "gBufferView"), m_GBufferView);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "pointMode"), m_PointMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "directionalMode"), m_DirectionalMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "iblMode"), m_IBLMode);
            glUniform1i(glGetUniformLocation(m_Shaders->LightingBRDF->GetHandle(), "attenuationMode"), m_AttenuationMode);

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
                m_SkyBox->Bind();
                glActiveTexture(GL_TEXTURE6);
                m_IBL->MapIrradiance.Bind();
                glActiveTexture(GL_TEXTURE7);
                m_IBL->MapPrefilter.Bind();
                glActiveTexture(GL_TEXTURE8);
                m_IBL->MapLUT.Bind();
            }

            /// Do Later
            std::vector<std::string> pointLightUniforms = { "lightPointArray[0].color", "lightPointArray[0].position", "lightPointArray[0].radius" };
            for (auto idPointPair : m_PointLightMap)
            {
                if (idPointPair.second)
                {
                    idPointPair.second->UpdateUniforms(pointLightUniforms, *m_Shaders->LightingBRDF, view);
                }
            }

            std::vector<std::string> directionLightUniforms = { "lightDirectionalArray[0].color", "lightDirectionalArray[0].direction" };
            for (auto idDirectionalPair : m_DirectionalLightMap)
            {
                if (idDirectionalPair.second)
                {
                    idDirectionalPair.second->UpdateUniforms(directionLightUniforms, *m_Shaders->LightingBRDF, view);
                }
            }



            m_Window->GetQuad()->Draw(false /*wireframe*/);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glQueryCounter(m_Profiler.QueryIDLighting[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void GraphicsEngine::PostProcessRendering(GLfloat deltaTime)
        {
            glQueryCounter(m_Profiler.QueryIDPostprocess[GPUProfiler::Start], GL_TIMESTAMP);

            glClear(GL_COLOR_BUFFER_BIT);
            m_Shaders->FirstPassPostProcess->Bind();

            if (!firstPassProcess)
            {
                glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "sao"), 1);
                glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "gEffects"), 2);
            }

            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "gBufferView"), m_GBufferView);
            glUniform2f(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "screenTextureSize"), 1.0f / m_Window->GetActiveState().Width, 1.0f / m_Window->GetActiveState().Height);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "cameraAperture"), m_Camera->GetAperture());
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "cameraShutterSpeed"), m_Camera->GetShutterSpeed());
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "cameraISO"), m_Camera->GetISO());
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "saoMode"), m_SAOMode);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "fxaaMode"), m_FXAAMode);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "motionBlurMode"), m_MotionBlurMode);
            glUniform1f(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "motionBlurScale"), deltaTime);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "motionBlurMaxSamples"), m_SAO->MotionBlurMaxSamples);
            glUniform1i(glGetUniformLocation(m_Shaders->FirstPassPostProcess->GetHandle(), "tonemappingMode"), m_ToneMappingMode);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_PostProcessBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_SAOBlurBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);

            m_Window->GetQuad()->Draw(false /*wireframe*/);

            glQueryCounter(m_Profiler.QueryIDPostprocess[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void GraphicsEngine::ForwardPassRendering(const mat4& view, const mat4& proj)
        {
            glQueryCounter(m_Profiler.QueryIDForward[GPUProfiler::Start], GL_TIMESTAMP);

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
                m_Shaders->Simple->Bind();
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->Simple->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
                glUniformMatrix4fv(glGetUniformLocation(m_Shaders->Simple->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));

                for (auto idPointPair : m_PointLightMap)
                {
                    if (idPointPair.second)
                    {
                        auto colour = idPointPair.second->GetColour();
                        glUniform4f(glGetUniformLocation(m_Shaders->Simple->GetHandle(), "lightColor"), colour.r, colour.g, colour.b, colour.a);
                    }
                }

            }
            glQueryCounter(m_Profiler.QueryIDForward[GPUProfiler::Stop], GL_TIMESTAMP);
        }

        void GraphicsEngine::ToggleWireframe()
        {
            m_WireframeMode = !m_WireframeMode;
        }

        void GraphicsEngine::ToggleSAO()
        {
            m_SAOMode = !m_SAOMode;
        }

        void GraphicsEngine::ToggleFXAA()
        {
            m_FXAAMode = !m_FXAAMode;
        }

        void GraphicsEngine::ToggleMotionBlur()
        {
            m_MotionBlurMode = !m_MotionBlurMode;
        }

        void GraphicsEngine::TogglePointLightRender()
        {
            m_PointMode = !m_PointMode;
        }

        void GraphicsEngine::ToggleDirectionalLightRender()
        {
            m_DirectionalMode = !m_DirectionalMode;
        }

        void GraphicsEngine::ToggleEnviromentLightRender()
        {
            m_IBLMode = !m_IBLMode;
        }

        void GraphicsEngine::ToggleToneMapping(unsigned int tone)
        {
            m_ToneMappingMode = tone;
        }

        void GraphicsEngine::SetSkyBox(std::shared_ptr<Texture> skyBox)
        {
            m_SkyBox = skyBox;
        }

        std::shared_ptr<Texture> GraphicsEngine::GetSkyBox() const
        {
            return m_SkyBox;
        }

        void GraphicsEngine::SetPointLightMap(std::map<unsigned int, std::shared_ptr<PointLight>> idPointMap)
        {
            m_PointLightMap = idPointMap;
        }

        const std::map<unsigned int, std::shared_ptr<PointLight>>& GraphicsEngine::GetPointLightMap() const
        {
            return m_PointLightMap;
        }

        void GraphicsEngine::SetDirectionalLightMap(std::map<unsigned int, std::shared_ptr<DirectionalLight>> idDirectionalMap)
        {
            m_DirectionalLightMap = idDirectionalMap;
        }

        const std::map<unsigned int, std::shared_ptr<DirectionalLight>>& GraphicsEngine::GetDirectionalLightMap() const
        {
            return m_DirectionalLightMap;
        }

        void GraphicsEngine::SetGLWindow(std::shared_ptr<GLWindow> window)
        {
            m_Window = window;
        }

        std::shared_ptr<GLWindow> GraphicsEngine::GetGLWindow()
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

        void GraphicsEngine::SetShaders(const std::shared_ptr<StandardShaders>& shaders)
        {
            m_Shaders = shaders;
        }

        const std::shared_ptr<StandardShaders>& GraphicsEngine::GetShaders() const
        {
            return m_Shaders;
        }

        void GraphicsEngine::SetGBufferFBO(GLuint fbo)
        {
            m_GBufferFBO = fbo;
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
}