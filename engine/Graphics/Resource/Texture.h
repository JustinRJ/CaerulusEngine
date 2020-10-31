#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Graphics/Pipeline/IBindable.h"

namespace Graphics
{
    namespace Resource
    {
        class CAERULUS_GRAPHICS Texture : public Pipeline::IBindable
        {
        public:
            Texture(const std::string& path);
            virtual ~Texture();

            void Bind(unsigned int slot) const override;
            void Unbind() const override;
            unsigned int GetHandle() const;

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
