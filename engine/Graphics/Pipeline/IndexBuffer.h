#pragma once

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class IndexBuffer : public Core::Interface::NonCopyable
        {
        public:
            IndexBuffer(const unsigned int* data, size_t count) :
                m_count(count)
            {
                glGenBuffers(1, &m_handle);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_count, data, GL_STATIC_DRAW);
            }

            ~IndexBuffer()
            {
                glDeleteBuffers(1, &m_handle);
            }

            void Bind() const
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            }

            static void Unbind()
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }

            unsigned int GetHandle() const
            {
                return m_handle;
            }

            size_t GetCount() const
            {
                return m_count;
            }

        private:
            unsigned int m_handle;
            size_t m_count;
        };
    }
}