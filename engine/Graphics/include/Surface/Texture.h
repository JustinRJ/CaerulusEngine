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
            Texture(const std::string& path, bool isHDR);
            ~Texture();

            unsigned int GetHandle() const;
            void Bind(GLuint slot) const;
            static void Unbind();

            void ComputeMipmap();

            int GetWidth() const;
            int GetHeight() const;
            
            bool IsLoaded() const;

            const std::string& GetPath() const;

        private:
            bool Load(const std::string& path);
            bool LoadHDR(const std::string& path);

            const std::string m_path;
            GLuint m_handle;

            int m_width;
            int m_height;
            int m_BPP;
            bool m_isLoaded;
        };
    }
}
