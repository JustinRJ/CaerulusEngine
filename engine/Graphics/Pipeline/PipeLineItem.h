#pragma once

#include "../PipeLine/Shader.h"
#include "../Resource/Texture.h"
#include <memory>

namespace Graphics
{
    namespace PipeLine
    {
        // TODO - turn into derived classes
        enum class Item
        {
            GBuffer,
            Effects,
            Cube
        };

        struct TextureItem
        {
            bool Wrap = false;
            int Type = GL_FLOAT;
            int Format = GL_RGBA;
            int InternalFormat = GL_RGBA16F;
            int TextureTarget = GL_TEXTURE_2D;
            int Attachment = GL_COLOR_ATTACHMENT0;
        };

        class PipeLineItem
        {
        public:
            PipeLineItem()
            {
            }

            virtual ~PipeLineItem() = default;

            virtual void SetUniform() = 0;
            virtual void UpdateUniforms() = 0;

            void SetShader(std::shared_ptr<Shader> shader)
            {
                m_Shader = shader;
            }

            std::shared_ptr<Shader> GetShader() const
            {
                return m_Shader;
            }

            void SetTexture(std::shared_ptr<Resource::Texture> texture)
            {
                m_Texture = texture;
            }

            std::shared_ptr<Resource::Texture> GetTexture() const
            {
                return m_Texture;
            }

        private:

            GLuint m_FBO = -1; // FBO groups RBOs and Textures
            std::vector<GLuint> m_RBOs;
            std::vector<GLuint> m_Textures;

            std::shared_ptr<Shader> m_Shader;
            std::shared_ptr<Resource::Texture> m_Texture;
        };
    }
}