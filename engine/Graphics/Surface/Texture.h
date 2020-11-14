#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Surface
    {
        class CAERULUS_GRAPHICS Texture : public Core::Interface::NonCopyable
        {
        public:
            Texture(const std::string& path);
            virtual ~Texture();

            unsigned int GetHandle() const;
            void Bind(unsigned int slot) const;
            static void Unbind();

            void ComputeMipmap();

            int GetWidth() const;
            int GetHeight() const;
            
            bool IsLoaded() const;

            const std::string& GetPath() const;

        private:
            unsigned int m_handle;
            std::string m_path;
            unsigned char* m_localBuffer;
            int m_width;
            int m_height;
            int m_BPP;
            bool m_isLoaded;
        };
    }
}
