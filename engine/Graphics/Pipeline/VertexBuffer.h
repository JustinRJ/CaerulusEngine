#pragma once

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class VertexBuffer : public Core::Interface::NonCopyable
        {
        public:
            VertexBuffer(const void* data, size_t size) :
                m_size(size)
            {
                glGenBuffers(1, &m_handle);
                glBindBuffer(GL_ARRAY_BUFFER, m_handle);
                glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
            }

            ~VertexBuffer()
            {
                glDeleteBuffers(1, &m_handle);
            }

            void Bind() const
            {
                glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            }

            static void Unbind()
            {
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }

            unsigned int GetHandle() const
            {
                return m_handle;
            }

            size_t GetSize() const
            {
                return m_size;
            }

        private:
            unsigned int m_handle;
            size_t m_size;
        };
    }
}