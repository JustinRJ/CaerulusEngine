#pragma once

#include "Graphics/Geometry/Cube.h"
#include "Graphics/Surface/Texture.h"
#include "Graphics/Rendering/IRenderer.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Lighting
    {
        class IBL : public Pipeline::ShaderUniformFunctor
        {
        public:
            IBL(std::shared_ptr<Pipeline::Shader> cubemapShader, std::shared_ptr<Surface::Texture> texture, std::shared_ptr<Rendering::IRenderer> renderer) :
                m_texture(texture),
                m_renderer(renderer)
            {
                // TODO - create frame buffer class
                unsigned int captureFBO;
                unsigned int captureRBO;
                glGenFramebuffers(1, &captureFBO);
                glGenRenderbuffers(1, &captureRBO);

                glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
                glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
                //-----------------------------------

                glGenTextures(1, &m_cubeHandle);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeHandle);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
                }
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                const Core::Math::mat4 captureProjection = Core::Math::perspective(Core::Math::radians(90.0f), 1.0f, 0.1f, 10.0f);
                const std::vector<Core::Math::mat4> captureViews =
                {
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(1.0f,  0.0f,  0.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(-1.0f,  0.0f,  0.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  1.0f,  0.0f), Core::Math::vec3(0.0f,  0.0f,  1.0f)),
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f), Core::Math::vec3(0.0f,  0.0f, -1.0f)),
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  0.0f,  1.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f)),
                    lookAt(Core::Math::vec3(0.0f, 0.0f, 0.0f), Core::Math::vec3(0.0f,  0.0f, -1.0f), Core::Math::vec3(0.0f, -1.0f,  0.0f))
                };

                cubemapShader->SetMat4fv("projection", captureProjection);

                m_texture->Bind(0);

                glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
                for (unsigned int i = 0; i < 6; ++i)
                {
                    cubemapShader->SetMat4fv("view", captureViews[i]);
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_cubeHandle, 0);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    m_renderer->Draw(m_cube);
                }
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

            void Draw() const
            {
                Surface::Texture::Unbind();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeHandle);
                m_renderer->Draw(m_cube);
            }

        private:
            unsigned int m_cubeHandle;
            Geometry::Cube m_cube;
            std::shared_ptr<Surface::Texture> m_texture;
            std::shared_ptr<Rendering::IRenderer> m_renderer;
        };
    }
}
