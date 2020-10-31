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
        Texture::Texture(const std::string& path) : 
            m_path(path),
            m_localBuffer(nullptr),
            m_width(0),
            m_height(0),
            m_BPP(0),
            m_isLoaded(false)
        {
            stbi_set_flip_vertically_on_load(1);
            m_localBuffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4 /*rgba*/);

            if (!m_localBuffer)
            {
                using Core::Logging::Log;
                Log::LogError("Failed to load texture with path: " + std::string(path));
                return;
            }

            m_isLoaded = true;

            glGenTextures(1, &m_handle);
            glBindTexture(GL_TEXTURE_2D, m_handle);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_localBuffer);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        Texture::~Texture()
        {
            if (m_localBuffer)
            {
                stbi_image_free(m_localBuffer);
            }
            glDeleteTextures(1, &m_handle);
        }

        unsigned int Texture::GetHandle() const
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

        const std::string& Texture::GetPath() const
        {
            return m_path;
        }

        void Texture::Bind(unsigned int slot) const
        {
            glActiveTexture(GL_TEXTURE0 + slot);
            glBindTexture(GL_TEXTURE_2D, m_handle);
        }

        void Texture::Unbind() const
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
