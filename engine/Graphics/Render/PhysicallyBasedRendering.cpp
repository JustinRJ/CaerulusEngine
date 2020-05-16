#include "stdafx.h"

#include "PhysicallyBasedRendering.h"
#include "../../Core/Time/Timer.h"
#include "../../Core/Math/MathHelper.h"
#include "../../Core/Logging/Log.h"
#include "../GraphicsEngine.h"
#include "../../Managers/Managers/ShaderManager.h"

namespace Graphics
{
    namespace Render
    {
        PhysicallyBasedRendering::PhysicallyBasedRendering(GraphicsEngine& graphicsEngine) :
            m_GraphicsEngine(graphicsEngine)
        {
            // IBL
            m_MapLUT = std::make_shared<Texture>();
            m_MapLUT->CreateHDR(512, 512, GL_RG, GL_RG16F, GL_FLOAT, GL_LINEAR);
            m_MapCube = std::make_shared<Texture>();
            m_MapCube->CreateCube(512, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_MapIrradiance = std::make_shared<Texture>();
            m_MapIrradiance->CreateCube(32, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR);
            m_MapPrefilter = std::make_shared<Texture>();
            m_MapPrefilter->CreateCube(128, GL_RGB, GL_RGB16F, GL_FLOAT, GL_LINEAR_MIPMAP_LINEAR);
            m_MapPrefilter->ComputeMipmap();

            mat4 sceneCubeTransform;
            MathHelper::CreateTansform(sceneCubeTransform, vec3(1.0f), vec3(0.0f), vec3(0.0f));
            m_SceneCube = std::make_shared<Cube>(sceneCubeTransform);

            m_MapProjection = perspective(radians(90.0f), 1.0f, 0.1f, 10.0f);
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(-1.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f)));
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f)));
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f)));
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, -1.0f, 0.0f)));
            m_MapView.push_back(lookAt(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, -1.0f, 0.0f)));

            glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            glDepthFunc(GL_LESS);
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        }

        void PhysicallyBasedRendering::Init()
        {
            Point = std::make_shared<PointLight>(LightingBRDF, m_GraphicsEngine.GetCamera());
            Point->SetPosition(vec3(0.0, 25.0f, 0.0f));
            Point->SetRadius(10000.0f);
            Point->UpdateUniformsCallback([&](PointLight& p)
            {
                std::vector<std::string> uniforms = { "lightPointArray[0].color", "lightPointArray[0].position", "lightPointArray[0].radius" };
                vec3 lightPositionViewSpace = vec3(p.GetCamera()->GetViewMatrix() * vec4(p.GetPosition(), 1.0f));
                p.GetShader()->Set4f(uniforms.at(0), p.GetColour());
                p.GetShader()->Set3f(uniforms.at(1), lightPositionViewSpace);
                p.GetShader()->Set1f(uniforms.at(2), p.GetRadius());
            });
            m_GraphicsEngine.SetPointLightMap({ std::make_pair(0, Point) });

            Directional = std::make_shared<DirectionalLight>(LightingBRDF, m_GraphicsEngine.GetCamera());
            Directional->SetDirection(vec3(0.0, -1.0f, 0.0f));
            Directional->UpdateUniformsCallback([&](DirectionalLight& d)
            {
               std::vector<std::string> uniforms = { "lightDirectionalArray[0].color", "lightDirectionalArray[0].direction" };
               vec3 lightDirectionViewSpace = vec3(d.GetCamera()->GetViewMatrix() * vec4(d.GetDirection(), 0.0f));
               d.GetShader()->Set4f(uniforms.at(0).c_str(), d.GetColour());
               d.GetShader()->Set3f(uniforms.at(1).c_str(), lightDirectionViewSpace);
            });
            m_GraphicsEngine.SetDirectionalLightMap({ std::make_pair(0, Directional) });

            /// ------------------------------------------------------------------------------------------------------------------------------
            /// GBUFFER
            /// ------------------------------------------------------------------------------------------------------------------------------
            glGenFramebuffers(1, &m_GBufferFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);

            // Position
            glGenTextures(1, &m_GPositionBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPositionBuffer, 0);

            // Albedo + Roughness
            glGenTextures(1, &m_GAlbedoBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GAlbedoBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GAlbedoBuffer, 0);

            // Normals + Metalness
            glGenTextures(1, &m_GNormalBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GNormalBuffer, 0);

            // Effects (AO + Velocity)
            glGenTextures(1, &m_GEffectsBuffer);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_GEffectsBuffer, 0);

            // Define the COLOR_ATTACHMENTS for the G-Buffer
            GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(4, attachments);

            // Z-Buffer
            glGenRenderbuffers(1, &m_DepthRBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_DepthRBO);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GetWindowWidth(), GetWindowHeight());
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, GetWindowWidth(), GetWindowHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, GetWindowWidth(), GetWindowHeight(), 0, GL_RGBA, GL_FLOAT, NULL);
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


            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_MapCube->GetWidth(), m_MapCube->GetHeight());
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_EnvToCubeRBO);
            LatlongToCube->Bind();
            glUniform1i(glGetUniformLocation(LatlongToCube->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(LatlongToCube->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_MapProjection));
            glActiveTexture(GL_TEXTURE0);
            SkyBox->Bind();
            glViewport(0, 0, m_MapCube->GetWidth(), m_MapCube->GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_EnvToCubeFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(LatlongToCube->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_MapCube->GetHandle(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_SceneCube->Draw(false /*wireframe*/);
            }
            m_MapCube->ComputeMipmap();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// Diffuse irradiance capture
            glGenFramebuffers(1, &m_IrradianceFBO);
            glGenRenderbuffers(1, &m_IrradianceRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_IrradianceRBO);


            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_MapIrradiance->GetWidth(), m_MapIrradiance->GetHeight());
            IrradianceIBL->Bind();
            glUniform1i(glGetUniformLocation(IrradianceIBL->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(IrradianceIBL->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_MapProjection));
            glActiveTexture(GL_TEXTURE0);
            m_MapCube->Bind();
            glViewport(0, 0, m_MapIrradiance->GetWidth(), m_MapIrradiance->GetHeight());
            glBindFramebuffer(GL_FRAMEBUFFER, m_IrradianceFBO);
            for (unsigned int i = 0; i < 6; ++i)
            {
                glUniformMatrix4fv(glGetUniformLocation(IrradianceIBL->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_MapView[i]));
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_MapIrradiance->GetHandle(), 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                m_SceneCube->Draw(false /*wireframe*/);
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// Prefilter cubemap
            PrefilterIBL->Bind();
            glUniform1i(glGetUniformLocation(PrefilterIBL->GetHandle(), "envMap"), 0);
            glUniformMatrix4fv(glGetUniformLocation(PrefilterIBL->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(m_MapProjection));
            m_MapCube->Bind();


            glGenFramebuffers(1, &m_PrefilterFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_PrefilterFBO);


            unsigned int maxMipLevels = 5;
            for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
            {
                GLsizei mipWidth = static_cast<GLsizei>(m_MapPrefilter->GetWidth() * std::pow(0.5, mip));
                GLsizei mipHeight = static_cast<GLsizei>(m_MapPrefilter->GetHeight() * std::pow(0.5, mip));

                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                glViewport(0, 0, mipWidth, mipHeight);

                float roughness = (float)mip / (float)(maxMipLevels - 1);

                glUniform1f(glGetUniformLocation(PrefilterIBL->GetHandle(), "roughness"), roughness);
                glUniform1f(glGetUniformLocation(PrefilterIBL->GetHandle(), "cubeResolutionWidth"), static_cast<GLfloat>(m_MapPrefilter->GetWidth()));
                glUniform1f(glGetUniformLocation(PrefilterIBL->GetHandle(), "cubeResolutionHeight"), static_cast<GLfloat>(m_MapPrefilter->GetHeight()));

                for (unsigned int i = 0; i < 6; ++i)
                {
                    glUniformMatrix4fv(glGetUniformLocation(PrefilterIBL->GetHandle(), "view"), 1, GL_FALSE, value_ptr(m_MapView[i]));
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_MapPrefilter->GetHandle(), mip);

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_SceneCube->Draw(false /*wireframe*/);
                }
            }
            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            /// BRDF LUT
            glGenFramebuffers(1, &m_BrdfLUTFBO);
            glGenRenderbuffers(1, &m_BrdfLUTRBO);
            glBindFramebuffer(GL_FRAMEBUFFER, m_BrdfLUTFBO);
            glBindRenderbuffer(GL_RENDERBUFFER, m_BrdfLUTRBO);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_MapLUT->GetWidth(), m_MapLUT->GetHeight());
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MapLUT->GetHandle(), 0);
            glViewport(0, 0, m_MapLUT->GetWidth(), m_MapLUT->GetHeight());
            IntegrateIBL->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




            m_GraphicsEngine.GetGLWindow()->GetQuad()->Draw(false /*wireframe*/);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, GetWindowWidth(), GetWindowHeight());
        }



        void PhysicallyBasedRendering::DrawAll(float deltaTime)
        {
            // geo rendering
            if (GBuffer)
            {
                GBuffer->Bind();

                if (!gBuffer)
                {
                    glUniform1i(glGetUniformLocation(GBuffer->GetHandle(), "texAlbedo"), 0);
                    glUniform1i(glGetUniformLocation(GBuffer->GetHandle(), "texNormal"), 1);
                    glUniform1i(glGetUniformLocation(GBuffer->GetHandle(), "texRoughness"), 2);
                    glUniform1i(glGetUniformLocation(GBuffer->GetHandle(), "texMetalness"), 3);
                    glUniform1i(glGetUniformLocation(GBuffer->GetHandle(), "texAO"), 4);
                    gBuffer = true;
                }
                
                float nearPlane = m_GraphicsEngine.GetCamera()->GetNear();
                float farPlane = m_GraphicsEngine.GetCamera()->GetFar();
                mat4 view = m_GraphicsEngine.GetCamera()->GetViewMatrix() /** this by transform of camera*/;
                mat4 proj = m_GraphicsEngine.GetCamera()->GetProjMatrix();
                mat4 cameraTransform;
                m_GraphicsEngine.GetCamera()->GetProjViewModel(cameraTransform);
                m_ProjViewModel = cameraTransform;
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "nearPlane"), 1, GL_FALSE, &nearPlane);
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "farPlane"), 1, GL_FALSE, &farPlane);
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "projViewModel"), 1, GL_FALSE, value_ptr(m_ProjViewModel));
                glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "projViewModelPrev"), 1, GL_FALSE, value_ptr(m_ProjViewModelPrev));
                
                for (auto& model : m_GraphicsEngine.GetModelMap())
                {
                    if (model.second)
                    {
                        const mat4& transform = *m_GraphicsEngine.GetTransformMap().at(model.first);
                        glUniformMatrix4fv(glGetUniformLocation(GBuffer->GetHandle(), "model"), 1, GL_FALSE, value_ptr(transform));
                        model.second->Draw(m_WireframeMode, m_GraphicsEngine.GetDefaultMaterial());
                    }
                }
            }
        }

        void PhysicallyBasedRendering::SwapBuffer(float deltaTime)
        {
            mat4 view = m_GraphicsEngine.GetCamera()->GetViewMatrix()/* * m_TransformCM->GetTransform(camera)*/;
            mat4 proj = m_GraphicsEngine.GetCamera()->GetProjMatrix();

            //update previous projviewmodel
            m_ProjViewModelPrev = m_ProjViewModel;

            SAORendering(proj);
            LightRendering(view, proj);
            PostProcessRendering(deltaTime);
            ForwardPassRendering(view, proj);
            m_GraphicsEngine.GetGLWindow()->SwapBuffer();
        }

        void PhysicallyBasedRendering::Clear()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_GBufferFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void PhysicallyBasedRendering::FlushErrors()
        {
            while (glGetError() != GL_NO_ERROR)
            {
            }
        }

        void PhysicallyBasedRendering::SAORendering(const mat4& proj)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_SAOFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            if (SAO && m_SAOMode)
            {
                // SAO noisy texture
                SAO->Bind();

                if (!sao)
                {
                    glUniform1i(glGetUniformLocation(SAO->GetHandle(), "gPosition"), 0);
                    glUniform1i(glGetUniformLocation(SAO->GetHandle(), "gNormal"), 1);
                }

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_GPositionBuffer);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_GNormalBuffer);

                glUniform1i(glGetUniformLocation(SAO->GetHandle(), "saoSamples"), Samples);
                glUniform1f(glGetUniformLocation(SAO->GetHandle(), "saoRadius"), Radius);
                glUniform1i(glGetUniformLocation(SAO->GetHandle(), "saoTurns"), Turns);
                glUniform1f(glGetUniformLocation(SAO->GetHandle(), "saoBias"), Bias);
                glUniform1f(glGetUniformLocation(SAO->GetHandle(), "saoScale"), Scale);
                glUniform1f(glGetUniformLocation(SAO->GetHandle(), "saoContrast"), Contrast);
                glUniform1i(glGetUniformLocation(SAO->GetHandle(), "viewportWidth"), GetWindowWidth());
                glUniform1i(glGetUniformLocation(SAO->GetHandle(), "viewportHeight"), GetWindowHeight());

                m_GraphicsEngine.GetGLWindow()->GetQuad()->Draw(false /*wireframe*/);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // SAO blur pass
                glBindFramebuffer(GL_FRAMEBUFFER, m_SAOBlurFBO);
                glClear(GL_COLOR_BUFFER_BIT);

                SAOBlur->Bind();

                glUniform1i(glGetUniformLocation(SAOBlur->GetHandle(), "saoBlurSize"), BlurSize);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_SAOBuffer);

                m_GraphicsEngine.GetGLWindow()->GetQuad()->Draw(false /*wireframe*/);

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }
        }

        void PhysicallyBasedRendering::LightRendering(const mat4& view, const mat4& proj)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_PostProcessFBO);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            LightingBRDF->Bind();

            if (!lightingBRDF)
            {
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "gPosition"), 0);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "gAlbedo"), 1);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "gNormal"), 2);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "gEffects"), 3);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "sao"), 4);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "envMap"), 5);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "envMapIrradiance"), 6);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "envMapPrefilter"), 7);
                glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "envMapLUT"), 8);
                lightingBRDF = true;
            }

            glUniformMatrix4fv(glGetUniformLocation(LightingBRDF->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(LightingBRDF->GetHandle(), "transposedView"), 1, GL_FALSE, value_ptr(transpose(view)));
            glUniformMatrix4fv(glGetUniformLocation(LightingBRDF->GetHandle(), "inverseProj"), 1, GL_FALSE, value_ptr(inverse(proj)));
            glUniform1f(glGetUniformLocation(LightingBRDF->GetHandle(), "materialRoughness"), MaterialRoughness);
            glUniform1f(glGetUniformLocation(LightingBRDF->GetHandle(), "materialMetallicity"), MaterialMetallicity);
            glUniform3f(glGetUniformLocation(LightingBRDF->GetHandle(), "materialF0"), MaterialF0.r, MaterialF0.g, MaterialF0.b);
            glUniform1f(glGetUniformLocation(LightingBRDF->GetHandle(), "ambientIntensity"), AmbientIntensity);
            glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "gBufferView"), m_GBufferView);
            glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "pointMode"), m_PointMode);
            glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "directionalMode"), m_DirectionalMode);
            glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "iblMode"), m_IBLMode);
            glUniform1i(glGetUniformLocation(LightingBRDF->GetHandle(), "attenuationMode"), m_AttenuationMode);

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
            if (SkyBox)
            {
                SkyBox->Bind();
                glActiveTexture(GL_TEXTURE6);
                m_MapIrradiance->Bind();
                glActiveTexture(GL_TEXTURE7);
                m_MapPrefilter->Bind();
                glActiveTexture(GL_TEXTURE8);
                m_MapLUT->Bind();
            }

            for (auto& idPointPair : m_GraphicsEngine.GetPointLightMap())
            {
                if (idPointPair.second)
                {
                    idPointPair.second->UpdateUniforms();
                }
            }

            for (auto& idDirectionalPair : m_GraphicsEngine.GetDirectionalLightMap())
            {
                if (idDirectionalPair.second)
                {
                    idDirectionalPair.second->UpdateUniforms();
                }
            }

            m_GraphicsEngine.GetGLWindow()->GetQuad()->Draw(false /*wireframe*/);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void PhysicallyBasedRendering::PostProcessRendering(GLfloat deltaTime)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            FirstPassPostProcess->Bind();

            if (!firstPassProcess)
            {
                glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "sao"), 1);
                glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "gEffects"), 2);
            }

            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "gBufferView"), m_GBufferView);
            glUniform2f(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "screenTextureSize"), 1.0f / GetWindowWidth(), 1.0f / GetWindowHeight());
            glUniform1f(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "cameraAperture"), m_GraphicsEngine.GetCamera()->GetAperture());
            glUniform1f(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "cameraShutterSpeed"), m_GraphicsEngine.GetCamera()->GetShutterSpeed());
            glUniform1f(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "cameraISO"), m_GraphicsEngine.GetCamera()->GetISO());
            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "saoMode"), m_SAOMode);
            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "fxaaMode"), m_FXAAMode);
            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "motionBlurMode"), m_MotionBlurMode);
            glUniform1f(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "motionBlurScale"), deltaTime);
            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "motionBlurMaxSamples"), MotionBlurMaxSamples);
            glUniform1i(glGetUniformLocation(FirstPassPostProcess->GetHandle(), "tonemappingMode"), m_ToneMappingMode);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_PostProcessBuffer);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, m_SAOBlurBuffer);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, m_GEffectsBuffer);

            m_GraphicsEngine.GetGLWindow()->GetQuad()->Draw(false /*wireframe*/);
        }

        void PhysicallyBasedRendering::ForwardPassRendering(const mat4& view, const mat4& proj)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBufferFBO);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

            // Copy the depth informations from the Geometry Pass into the default framebuffer
            glBlitFramebuffer(0, 0, GetWindowWidth(), GetWindowHeight(),
                0, 0, GetWindowWidth(), GetWindowHeight(), GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            if (m_PointMode)
            {
                Simple->Bind();
                glUniformMatrix4fv(glGetUniformLocation(Simple->GetHandle(), "projection"), 1, GL_FALSE, value_ptr(proj));
                glUniformMatrix4fv(glGetUniformLocation(Simple->GetHandle(), "view"), 1, GL_FALSE, value_ptr(view));

                for (auto& idPointPair : m_GraphicsEngine.GetPointLightMap())
                {
                    if (idPointPair.second)
                    {
                        auto colour = idPointPair.second->GetColour();
                        glUniform4f(glGetUniformLocation(Simple->GetHandle(), "lightColor"), colour.r, colour.g, colour.b, colour.a);
                    }
                }
            }
        }

        void PhysicallyBasedRendering::ToggleWireframe()
        {
            m_WireframeMode = !m_WireframeMode;
        }

        void PhysicallyBasedRendering::ToggleSAO()
        {
            m_SAOMode = !m_SAOMode;
        }

        void PhysicallyBasedRendering::ToggleFXAA()
        {
            m_FXAAMode = !m_FXAAMode;
        }

        void PhysicallyBasedRendering::ToggleMotionBlur()
        {
            m_MotionBlurMode = !m_MotionBlurMode;
        }

        void PhysicallyBasedRendering::TogglePointLights()
        {
            m_PointMode = !m_PointMode;
        }

        void PhysicallyBasedRendering::ToggleDirectionalLights()
        {
            m_DirectionalMode = !m_DirectionalMode;
        }

        void PhysicallyBasedRendering::ToggleEnviromentLights()
        {
            m_IBLMode = !m_IBLMode;
        }

        void PhysicallyBasedRendering::ToggleToneMapping(unsigned int tone)
        {
            m_ToneMappingMode = tone;
        }

        int PhysicallyBasedRendering::GetWindowWidth() const
        {
            return m_GraphicsEngine.GetGLWindow()->GetActiveState().Width;
        }

        int PhysicallyBasedRendering::GetWindowHeight() const
        {
            return m_GraphicsEngine.GetGLWindow()->GetActiveState().Height;
        }
    }
}