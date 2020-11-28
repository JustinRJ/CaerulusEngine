#pragma once

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class VertexBuffer : public Core::Interface::NonCopyable
        {
        public:
            VertexBuffer(const void* data, size_t size);
            ~VertexBuffer();

            void Bind() const;
            static void Unbind();

            GLuint GetHandle() const;
            size_t GetSize() const;

        private:
            GLuint m_handle;
            static GLuint m_boundHandle;
            size_t m_size;
        };
    }
}