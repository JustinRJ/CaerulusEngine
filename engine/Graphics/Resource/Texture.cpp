#include "stdafx.h"

#include "Texture.h"
//required before stb_image define
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include <iostream>

namespace Graphics
{
    namespace Resource
    {
        Texture::Texture()
        {
        }

        Texture::~Texture()
        {
            glDeleteTextures(1, &m_ID);
        }

        GLint Texture::GetID()
        {
            return m_ID;
        }

        bool Texture::Load(const char* texPath, bool texFlip)
        {
            m_Type = GL_TEXTURE_2D;

            stbi_set_flip_vertically_on_load(texFlip);

            glGenTextures(1, &m_ID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_ID);
            // Request the maximum level of anisotropy the GPU used can support and use it
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_AnisoFilterLevel);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_AnisoFilterLevel);

            int width, height, numComponents;
            unsigned char* texData = stbi_load(texPath, &width, &height, &numComponents, 0);

            m_Width = width;
            m_Height = height;
            m_Components = numComponents;
            m_Path = texPath;

            if (texData)
            {
                if (numComponents == 1)
                {
                    m_Format = GL_RED;
                }
                else if (numComponents == 3)
                {
                    m_Format = GL_RGB;
                }
                else if (numComponents == 4)
                {
                    m_Format = GL_RGBA;
                }
                m_InternalFormat = m_Format;

                glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, texData);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // Need AF to get ride of the blur on textures
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                std::cerr << "Failed to load texture: " << texPath << std::endl;
                return false;
            }

            stbi_image_free(texData);

            glBindTexture(GL_TEXTURE_2D, 0);
            return true;
        }

        bool Texture::LoadHDR(const char* texPath, bool texFlip)
        {
            m_Type = GL_TEXTURE_2D;

            stbi_set_flip_vertically_on_load(texFlip);

            glGenTextures(1, &m_ID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_ID);

            if (stbi_is_hdr(texPath))
            {
                int width, height, numComponents;
                float* texData = stbi_loadf(texPath, &width, &height, &numComponents, 0);

                m_Width = width;
                m_Height = height;
                m_Components = numComponents;
                m_Path = texPath;

                if (texData)
                {
                    // Need a higher precision format for HDR to not lose informations, thus 32bits floating point
                    if (numComponents == 3)
                    {
                        m_InternalFormat = GL_RGB32F;
                        m_Format = GL_RGB;
                    }
                    else if (numComponents == 4)
                    {
                        m_InternalFormat = GL_RGBA32F;
                        m_Format = GL_RGBA;
                    }

                    glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_FLOAT, texData);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    std::cerr << "Failed to load HDR texture: " << texPath << std::endl;
                    return false;
                }
                stbi_image_free(texData);
            }
            else
            {
                std::cerr << "Texture isnt a HDR texture: " << texPath << std::endl;
                return false;
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            return true;
        }

        void Texture::CreateHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
        {
            m_Type = GL_TEXTURE_2D;

            glGenTextures(1, &m_ID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_ID);

            m_Width = width;
            m_Height = height;
            m_Format = format;
            m_InternalFormat = internalFormat;

            if (format == GL_RED)
            {
                m_Components = 1;
            }
            else if (format == GL_RG)
            {
                m_Components = 2;
            }
            else if (format == GL_RGB)
            {
                m_Components = 3;
            }
            else if (format == GL_RGBA)
            {
                m_Components = 4;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLfloat>(minFilter));
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture::CreateCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
        {
            m_Type = GL_TEXTURE_CUBE_MAP;

            glGenTextures(1, &m_ID);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(m_Type, m_ID);

            for (GLuint i = 0; i < 6; ++i)
            {
                if (m_Width == 0 && m_Height == 0 && m_Components == 0)
                {
                    m_Width = width;
                    m_Height = width;
                    m_Format = format;
                    m_InternalFormat = internalFormat;
                }

                if (format == GL_RED)
                {
                    m_Components = 1;
                }
                else if (format == GL_RGB)
                {
                    m_Components = 3;
                }
                else if (format == GL_RGBA)
                {
                    m_Components = 4;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, m_Width, m_Height, 0, m_Format, type, nullptr);
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(m_Type, 0);
        }

        GLuint Texture::GetWidth() const
        {
            return m_Width;
        }

        GLuint Texture::GetHeight() const
        {
            return m_Height;
        }

        const std::string& Texture::GetPath() const
        {
            return m_Path;
        }

        void Texture::UseTexture() const
        {
            glBindTexture(m_Type, m_ID);
        }

        void Texture::ComputeMipmap()
        {
            glBindTexture(m_Type, m_ID);
            glGenerateMipmap(m_Type);
        }
    }
}
