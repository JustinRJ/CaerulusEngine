#include "stdafx.h"

#include "Surface/Texture.h"

#include <glew.h>
//required before stb_image define
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Log/Log.h"

using namespace Core::Log;

namespace Graphics
{
    namespace Surface
    {
        Texture::Texture(std::string_view path, bool isHDR) :
            m_path(path),
            m_width(0),
            m_height(0),
            m_BPP(0),
            m_isLoaded(false)
        {
            stbi_set_flip_vertically_on_load(true);
            glGenTextures(1, &m_handle);
            glBindTexture(GL_TEXTURE_2D, m_handle);
            m_isLoaded = isHDR ? LoadHDR(path) : Load(path);
            glBindTexture(GL_TEXTURE_2D, 0);

            if (!m_isLoaded)
            {
                LogError("Failed to load texture with path: " + std::string(path));
            }
        }

        Texture::~Texture()
        {
            glDeleteTextures(1, &m_handle);
        }

        bool Texture::Load(std::string_view path)
        {
            bool isLoaded = false;
            if (unsigned char* data = stbi_load(path.data(), &m_width, &m_height, &m_BPP, 0))
            {
                GLenum format = 0;
                if (m_BPP == 1)
                {
                    format = GL_RED;
                }
                else if (m_BPP == 3)
                {
                    format = GL_RGB;
                }
                else if (m_BPP == 4)
                {
                    format = GL_RGBA;
                }
                else
                {
                    LogError("Unsupported texture format!");
                }

                glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT /*GL_CLAMP_TO_EDGE*/);
                stbi_image_free(data);
                isLoaded = true;
            }
            return isLoaded;
        }

        bool Texture::LoadHDR(std::string_view path)
        {
            bool isLoaded = false;
            if (float* data = stbi_loadf(path.data(), &m_width, &m_height, &m_BPP, 0))
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                stbi_image_free(data);
                isLoaded = true;
            }
            return isLoaded;
        }

        uint32_t Texture::GetHandle() const
        {
            return m_handle;
        }

        int Texture::GetWidth() const
        {
            return m_width;
        }

        int Texture::GetHeight() const
        {
            return m_height;
        }

        std::string_view Texture::GetPath() const
        {
            return m_path;
        }

        void Texture::Bind(GLuint slot) const
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_handle);
        }

        void Texture::Unbind()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture::ComputeMipmap()
        {
            glBindTexture(GL_TEXTURE_2D, m_handle);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        bool Texture::IsLoaded() const
        {
            return m_isLoaded;
        }
    }
}
