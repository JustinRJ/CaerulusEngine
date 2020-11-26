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
            ~Texture();

            unsigned int GetHandle() const;
            void Bind(GLuint slot) const;
            void Unbind() const;

            void ComputeMipmap();

            int GetWidth() const;
            int GetHeight() const;
            
            bool IsLoaded() const;

            const std::string& GetPath() const;

        private:
            const std::string m_path;
            GLuint m_handle;
            static GLuint m_boundHandle;
            static GLuint m_boundSlot;

            unsigned char* m_localBuffer;
            int m_width;
            int m_height;
            int m_BPP;
            bool m_isLoaded;
        };
    }
}
