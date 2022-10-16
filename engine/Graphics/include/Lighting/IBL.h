#pragma once

#include "Math/Camera.h"
#include "Geometry/Cube.h"
#include "Surface/Texture.h"
#include "Surface/Material.h"
#include "Window/GLWindow.h"
#include "Rendering/IRenderer.h"
#include "Pipeline/FrameBuffer.h"
#include "Pipeline/ShaderUniformCallback.h"
#include "AssetManagers/ShaderManager.h"

namespace Graphics
{
    namespace Lighting
    {
        struct IBLShaders
        {
            std::string PbrShader;
            std::string CubemapShader;
            std::string IrradianceShader;
            std::string PrefilterShader;
            std::string BrdfShader;
            std::string BackgroundShader;
        };

        // TODO - re-write this, store on CameraComponent
        class IBL : public Pipeline::ShaderUniformCallback
        {
        public:
            IBL(const AssetManagers::ShaderManager& shaderManager,
                const AssetManagers::TextureManager& textureManager,
                const IBLShaders& iblShaders,
                std::string_view textureName,
                Rendering::IRenderer* renderer,
                Window::GLWindow* window,
                Core::Math::Camera* camera,
                Pipeline::FrameBuffer* framebuffer) :
                m_textureName(textureName)
            {
                m_cube.SetRenderer(renderer);

                uint32_t windowWidth = window->GetActiveState().Width;
                uint32_t windowHeight = window->GetActiveState().Height;

                // pbr: setup cubemap to render to and attach to framebuffer
                // ---------------------------------------------------------
                glGenTextures(1, &m_envCubemapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                for (uint32_t i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
                // ----------------------------------------------------------------------------------------------
                Core::Math::mat4 captureProjection = Core::Math::perspective(Core::Math::radians(90.0f), 1.0f, 0.1f, 10.0f);
                Core::Math::mat4 captureViews[] =
                {
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(1.0f,  0.0f,  0.0f),  Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(-1.0f,  0.0f,  0.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  1.0f,  0.0f),  Core::Math::vec3(0.0f,  0.0f,  1.0f)),
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f),  Core::Math::vec3(0.0f,  0.0f, -1.0f)),
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  0.0f,  1.0f),  Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    Core::Math::lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  0.0f, -1.0f),  Core::Math::vec3(0.0f, -1.0f,  0.0f))
                };

                std::shared_ptr<Pipeline::Shader> CubemapShader = shaderManager.Get(iblShaders.CubemapShader);

                // pbr: convert HDR equirectangular environment map to cubemap equivalent
                // ----------------------------------------------------------------------
                CubemapShader->Bind();
                CubemapShader->Set1i("equirectangularMap", 0);
                CubemapShader->SetMat4fv("projection", captureProjection);

                glActiveTexture(GL_TEXTURE0);

                std::shared_ptr<Surface::Texture> texture = textureManager.Get(m_textureName);
                glBindTexture(GL_TEXTURE_2D, texture->GetHandle());

                glViewport(0, 0, 512, 512);
                framebuffer->Bind();

                for (uint32_t i = 0; i < 6; ++i)
                {
                    CubemapShader->SetMat4fv("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubemapHandle, 0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_cube.Draw();
                }
                framebuffer->Unbind();

                // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

                // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
                // --------------------------------------------------------------------------------
                glGenTextures(1, &m_irradianceMapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                for (uint32_t i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                framebuffer->Bind();
                glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

                std::shared_ptr<Pipeline::Shader> IrradianceShader = shaderManager.Get(iblShaders.IrradianceShader);

                // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
                // -----------------------------------------------------------------------------
                IrradianceShader->Bind();
                IrradianceShader->Set1i("environmentMap", 0);
                IrradianceShader->SetMat4fv("projection", captureProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);

                glViewport(0, 0, windowWidth, windowHeight);
                framebuffer->Bind();
                for (uint32_t i = 0; i < 6; ++i)
                {
                    IrradianceShader->SetMat4fv("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMapHandle, 0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_cube.Draw();
                }
                framebuffer->Unbind();

                // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
                // --------------------------------------------------------------------------------
                glGenTextures(1, &m_prefilterMapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                for (uint32_t i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // be sure to set minification filter to mip_linear 
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // generate mipmaps for the cubemap so OpenGL automatically allocates the required memory.
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

                std::shared_ptr<Pipeline::Shader> PrefilterShader = shaderManager.Get(iblShaders.PrefilterShader);

                // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
                // ----------------------------------------------------------------------------------------------------
                PrefilterShader->Bind();
                PrefilterShader->Set1i("environmentMap", 0);
                PrefilterShader->SetMat4fv("projection", captureProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);

                framebuffer->Bind();
                uint32_t maxMipLevels = 5;
                for (uint32_t mip = 0; mip < maxMipLevels; ++mip)
                {
                    // resize framebuffer according to mip-level size.
                    uint32_t mipWidth = static_cast<uint32_t>(128 * std::pow(0.5, mip));
                    uint32_t mipHeight = static_cast<uint32_t>(128 * std::pow(0.5, mip));
                    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                    glViewport(0, 0, mipWidth, mipHeight);

                    float roughness = (float)mip / (float)(maxMipLevels - 1);
                    PrefilterShader->Set1f("roughness", roughness);
                    for (uint32_t i = 0; i < 6; ++i)
                    {
                        PrefilterShader->SetMat4fv("view", captureViews[i]);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMapHandle, mip);

                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        m_cube.Draw();
                    }
                }
                framebuffer->Unbind();

                // pbr: generate a 2D LUT from the BRDF equations used.
                // ----------------------------------------------------
                glGenTextures(1, &m_brdfLUTTextureHandle);

                // pre-allocate enough memory for the LUT texture.
                glBindTexture(GL_TEXTURE_2D, m_brdfLUTTextureHandle);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, windowWidth, windowHeight, 0, GL_RG, GL_FLOAT, 0);
                // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
                framebuffer->Bind();
                glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTextureHandle, 0);

                std::shared_ptr<Pipeline::Shader> BrdfShader = shaderManager.Get(iblShaders.BrdfShader);
                std::shared_ptr<Pipeline::Shader> PbrShader = shaderManager.Get(iblShaders.PbrShader);
                std::shared_ptr<Pipeline::Shader> BackgroundShader = shaderManager.Get(iblShaders.BackgroundShader);

                glViewport(0, 0, windowWidth, windowHeight);
                BrdfShader->Bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                window->Update();

                framebuffer->Unbind();

                // initialize static shader uniforms before rendering
                // --------------------------------------------------
                PbrShader->Bind();
                PbrShader->Set1i("irradianceMap", 0);
                PbrShader->Set1i("prefilterMap", 1);
                PbrShader->Set1i("brdfLUT", 2);
                PbrShader->SetMat4fv("projection", camera->GetPerspective().GetMatrix());
                BackgroundShader->Bind();
                BackgroundShader->Set1i("environmentMap", 0);
                BackgroundShader->SetMat4fv("projection", camera->GetPerspective().GetMatrix());

                glViewport(0, 0, windowWidth, windowHeight);

                std::cout << framebuffer->GetStatus() << std::endl;

                Surface::Material::SetMaterialTextureSlotOffset(GetRequiredTextureSlots());
            }

            const Geometry::Cube& GetCube() const
            {
                return m_cube;
            }

            std::string_view GetTexture() const
            {
                return m_textureName;
            }

            void Bind()
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, m_brdfLUTTextureHandle);
            }

            void Draw()
            {
                Surface::Texture::Unbind();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                m_cube.Draw();
            }

            uint32_t GetRequiredTextureSlots() const
            {
                // We require 3 texture slots
                // See Bind || Draw
                return 3;
            }

        private:
            uint32_t m_envCubemapHandle = 0;
            uint32_t m_irradianceMapHandle = 0;
            uint32_t m_prefilterMapHandle = 0;
            uint32_t m_brdfLUTTextureHandle = 0;

            Geometry::Cube m_cube;
            std::string m_textureName;
        };
    }
}
