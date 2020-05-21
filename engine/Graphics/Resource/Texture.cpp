#include "stdafx.h"

#include "Texture.h"
//required before stb_image define
#define STB_IMAGE_IMPLEMENTATION 
#include "stb_image.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Resource
    {
        Texture::~Texture()
        {
            glDeleteTextures(1, &m_handle);
        }

        GLint Texture::GetHandle()
        {
            return m_handle;
        }

        bool Texture::Load(const char* texPath, bool texFlip)
        {
            m_type = GL_TEXTURE_2D;

            stbi_set_flip_vertically_on_load(texFlip);

            glGenTextures(1, &m_handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_handle);
            // Request the maximum level of anisotropy the GPU used can support and use it
            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_anisoFilterLevel);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_anisoFilterLevel);

            int width, height, numComponents;
            unsigned char* texData = stbi_load(texPath, &width, &height, &numComponents, 0);

            m_width = width;
            m_height = height;
            m_components = numComponents;
            m_path = texPath;

            if (texData)
            {
                if (numComponents == 1)
                {
                    m_format = GL_RED;
                }
                else if (numComponents == 3)
                {
                    m_format = GL_RGB;
                }
                else if (numComponents == 4)
                {
                    m_format = GL_RGBA;
                }
                m_internalFormat = m_format;

                glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_UNSIGNED_BYTE, texData);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                // Need AF to get ride of the blur on textures
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                glGenerateMipmap(GL_TEXTURE_2D);
            }
            else
            {
                using Core::Logging::Log;
                Log::LogError("Failed to load texture: " + std::string(texPath));
                return false;
            }

            stbi_image_free(texData);

            glBindTexture(GL_TEXTURE_2D, 0);
            return true;
        }

        bool Texture::LoadHDR(const char* texPath, bool texFlip)
        {
            m_type = GL_TEXTURE_2D;

            stbi_set_flip_vertically_on_load(texFlip);

            glGenTextures(1, &m_handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_handle);

            if (stbi_is_hdr(texPath))
            {
                int width, height, numComponents;
                float* texData = stbi_loadf(texPath, &width, &height, &numComponents, 0);

                m_width = width;
                m_height = height;
                m_components = numComponents;
                m_path = texPath;

                if (texData)
                {
                    // Need a higher precision format for HDR to not lose informations, thus 32bits floating point
                    if (numComponents == 3)
                    {
                        m_internalFormat = GL_RGB32F;
                        m_format = GL_RGB;
                    }
                    else if (numComponents == 4)
                    {
                        m_internalFormat = GL_RGBA32F;
                        m_format = GL_RGBA;
                    }

                    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_FLOAT, texData);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else
                {
                    using Core::Logging::Log;
                    Log::LogError("Failed to load HDR texture: " + std::string(texPath));
                    return false;
                }
                stbi_image_free(texData);
            }
            else
            {
                using Core::Logging::Log;
                Log::LogError("Texture isnt a HDR texture: " + std::string(texPath));
                return false;
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            return true;
        }

        void Texture::CreateHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
        {
            m_type = GL_TEXTURE_2D;

            glGenTextures(1, &m_handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_handle);

            m_width = width;
            m_height = height;
            m_format = format;
            m_internalFormat = internalFormat;

            if (format == GL_RED)
            {
                m_components = 1;
            }
            else if (format == GL_RG)
            {
                m_components = 2;
            }
            else if (format == GL_RGB)
            {
                m_components = 3;
            }
            else if (format == GL_RGBA)
            {
                m_components = 4;
            }

            glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_format, GL_FLOAT, nullptr);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLfloat>(minFilter));
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);

            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture::CreateCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter)
        {
            m_type = GL_TEXTURE_CUBE_MAP;

            glGenTextures(1, &m_handle);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(m_type, m_handle);

            for (GLuint i = 0; i < 6; ++i)
            {
                if (m_width == 0 && m_height == 0 && m_components == 0)
                {
                    m_width = width;
                    m_height = width;
                    m_format = format;
                    m_internalFormat = internalFormat;
                }

                if (format == GL_RED)
                {
                    m_components = 1;
                }
                else if (format == GL_RGB)
                {
                    m_components = 3;
                }
                else if (format == GL_RGBA)
                {
                    m_components = 4;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_internalFormat, m_width, m_height, 0, m_format, type, nullptr);
            }

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            glBindTexture(m_type, 0);
        }

        GLuint Texture::GetWidth() const
        {
            return m_width;
        }

        GLuint Texture::GetHeight() const
        {
            return m_height;
        }

        const std::string& Texture::GetPath() const
        {
            return m_path;
        }

        void Texture::Bind() const
        {
            glBindTexture(m_type, m_handle);
        }

        void Texture::ComputeMipmap()
        {
            glBindTexture(m_type, m_handle);
            glGenerateMipmap(m_type);
        }
    }
}
