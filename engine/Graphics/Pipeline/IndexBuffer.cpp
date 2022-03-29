#include "stdafx.h"

#include "Pipeline\IndexBuffer.h"

namespace Graphics
{
    namespace Pipeline
    {
        IndexBuffer::IndexBuffer(const unsigned int* data, size_t count) :
            m_count(count)
        {
            glGenBuffers(1, &m_handle);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_count, data, GL_STATIC_DRAW);
        }

        IndexBuffer::~IndexBuffer()
        {
            glDeleteBuffers(1, &m_handle);
        }

        void IndexBuffer::Bind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
        }

        void IndexBuffer::Unbind()
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        GLuint IndexBuffer::GetHandle() const
        {
            return m_handle;
        }

        size_t IndexBuffer::GetCount() const
        {
            return m_count;
        }
    }
}