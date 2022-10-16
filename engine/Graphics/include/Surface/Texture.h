#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <string>
#include <glew.h>

#include "Interface/NonCopyable.h"

namespace Graphics
{
    namespace Surface
    {
        class CAERULUS_GRAPHICS Texture : Core::Interface::NonCopyable
        {
        public:
            Texture(std::string_view path, bool isHDR);
            ~Texture();

            uint32_t GetHandle() const;
            void Bind(GLuint slot) const;
            static void Unbind();

            void ComputeMipmap();

            int GetWidth() const;
            int GetHeight() const;
            
            bool IsLoaded() const;

            std::string_view GetPath() const;

        private:
            bool Load(std::string_view path);
            bool LoadHDR(std::string_view path);

            const std::string m_path;
            GLuint m_handle;

            int m_width;
            int m_height;
            int m_BPP;
            bool m_isLoaded;
        };
    }
}
