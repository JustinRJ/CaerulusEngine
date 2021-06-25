#pragma once

#include "Core/Math/Camera.h"
#include "Graphics/Geometry/Cube.h"
#include "Graphics/Surface/Texture.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Rendering/IRenderer.h"
#include "Graphics/Pipeline/FrameBuffer.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Lighting
    {
        struct IBLShaders
        {
            std::shared_ptr<Pipeline::Shader> PbrShader;
            std::shared_ptr<Pipeline::Shader> CubemapShader;
            std::shared_ptr<Pipeline::Shader> IrradianceShader;
            std::shared_ptr<Pipeline::Shader> PrefilterShader;
            std::shared_ptr<Pipeline::Shader> BrdfShader;
            std::shared_ptr<Pipeline::Shader> BackgroundShader;
        };

        class IBL : public Pipeline::ShaderUniformFunctor
        {
        public:
            IBL(IBLShaders iblShaders,
                std::shared_ptr<Surface::Texture> texture,
                std::shared_ptr<Rendering::IRenderer> renderer,
                std::shared_ptr<Window::GLWindow> window,
                std::shared_ptr<Core::Math::Camera> camera,
                std::shared_ptr<Pipeline::FrameBuffer> framebuffer) :
                m_texture(texture),
                m_renderer(renderer)
            {
                 // pbr: setup cubemap to render to and attach to framebuffer
                // ---------------------------------------------------------
                glGenTextures(1, &m_envCubemapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                for (unsigned int i = 0; i < 6; ++i)
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
                glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
                glm::mat4 captureViews[] =
                {
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
                    glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
                };

                // pbr: convert HDR equirectangular environment map to cubemap equivalent
                // ----------------------------------------------------------------------
                iblShaders.CubemapShader->Bind();
                iblShaders.CubemapShader->Set1i("equirectangularMap", 0);
                iblShaders.CubemapShader->SetMat4fv("projection", captureProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_texture->GetHandle());

                glViewport(0, 0, 512, 512); // TODO - don't forget to configure the viewport to the capture dimensions.
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetID());
                for (unsigned int i = 0; i < 6; ++i)
                {
                    iblShaders.CubemapShader->SetMat4fv("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_envCubemapHandle, 0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_renderer->Draw(m_cube, false);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // then let OpenGL generate mipmaps from first mip face (combatting visible dots artifact)
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

                // pbr: create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
                // --------------------------------------------------------------------------------
                glGenTextures(1, &m_irradianceMapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetID());
                glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);

                // pbr: solve diffuse integral by convolution to create an irradiance (cube)map.
                // -----------------------------------------------------------------------------
                iblShaders.IrradianceShader->Bind();
                iblShaders.IrradianceShader->Set1i("environmentMap", 0);
                iblShaders.IrradianceShader->SetMat4fv("projection", captureProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);

                glViewport(0, 0, 32, 32); // don't forget to configure the viewport to the capture dimensions.
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetID());
                for (unsigned int i = 0; i < 6; ++i)
                {
                    iblShaders.IrradianceShader->SetMat4fv("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMapHandle, 0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_renderer->Draw(m_cube, false);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // pbr: create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
                // --------------------------------------------------------------------------------
                glGenTextures(1, &m_prefilterMapHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                for (unsigned int i = 0; i < 6; ++i)
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

                // pbr: run a quasi monte-carlo simulation on the environment lighting to create a prefilter (cube)map.
                // ----------------------------------------------------------------------------------------------------
                iblShaders.PrefilterShader->Bind();
                iblShaders.PrefilterShader->Set1i("environmentMap", 0);
                iblShaders.PrefilterShader->SetMat4fv("projection", captureProjection);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);

                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetID());
                unsigned int maxMipLevels = 5;
                for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
                {
                    // reisze framebuffer according to mip-level size.
                    unsigned int mipWidth = static_cast<unsigned int>(128 * std::pow(0.5, mip));
                    unsigned int mipHeight = static_cast<unsigned int>(128 * std::pow(0.5, mip));
                    glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
                    glViewport(0, 0, mipWidth, mipHeight);

                    float roughness = (float)mip / (float)(maxMipLevels - 1);
                    iblShaders.PrefilterShader->Set1f("roughness", roughness);
                    for (unsigned int i = 0; i < 6; ++i)
                    {
                        iblShaders.PrefilterShader->SetMat4fv("view", captureViews[i]);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilterMapHandle, mip);

                        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                        m_renderer->Draw(m_cube, false);
                    }
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // pbr: generate a 2D LUT from the BRDF equations used.
                // ----------------------------------------------------
                glGenTextures(1, &m_brdfLUTTextureHandle);

                // pre-allocate enough memory for the LUT texture.
                glBindTexture(GL_TEXTURE_2D, m_brdfLUTTextureHandle);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
                // be sure to set wrapping mode to GL_CLAMP_TO_EDGE
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // then re-configure capture framebuffer object and render screen-space quad with BRDF shader.
                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetID());
                glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->GetDepthID());
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_brdfLUTTextureHandle, 0);

                glViewport(0, 0, 512, 512);
                iblShaders.BrdfShader->Bind();
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                window->Update();

                glBindFramebuffer(GL_FRAMEBUFFER, 0);

                // initialize static shader uniforms before rendering
                // --------------------------------------------------
                iblShaders.PbrShader->Bind();
                iblShaders.PbrShader->Set1i("irradianceMap", 0);
                iblShaders.PbrShader->Set1i("prefilterMap", 1);
                iblShaders.PbrShader->Set1i("brdfLUT", 2);
                iblShaders.PbrShader->SetMat4fv("projection", camera->GetPerspective().GetMatrix());
                iblShaders.BackgroundShader->Bind();
                iblShaders.BackgroundShader->Set1i("environmentMap", 0);
                iblShaders.BackgroundShader->SetMat4fv("projection", camera->GetPerspective().GetMatrix());

                glViewport(0, 0, window->GetActiveState().Width, window->GetActiveState().Height);
            }

            ~IBL() = default;

            const Geometry::Cube& GetCube() const
            {
                return m_cube;
            }

            std::shared_ptr<Surface::Texture> GetTexture() const
            {
                return m_texture;
            }

            void Bind() const
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_2D, m_brdfLUTTextureHandle);
            }

            void Draw() const
            {
                Surface::Texture::Unbind();
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_envCubemapHandle);
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_irradianceMapHandle);
                glActiveTexture(GL_TEXTURE2);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_prefilterMapHandle);
                m_renderer->Draw(m_cube);
            }

            unsigned int GetRequiredTextureSlots() const
            {
                // We require 3 texture slots
                // See Bind || Draw
                return 3;
            }

        private:
            unsigned int m_envCubemapHandle;
            unsigned int m_irradianceMapHandle;
            unsigned int m_prefilterMapHandle;
            unsigned int m_brdfLUTTextureHandle;

            Geometry::Cube m_cube;
            std::shared_ptr<Surface::Texture> m_texture;
            std::shared_ptr<Rendering::IRenderer> m_renderer;
        };
    }
}
