#include "stdafx.h"

#include "Pipeline\IndexBuffer.h"

namespace Graphics
{
    namespace Pipeline
    {
        GLuint IndexBuffer::m_boundHandle = 0;

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
            if (m_boundHandle != m_handle)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle);
                m_boundHandle = m_handle;
            }
        }

        void IndexBuffer::Unbind()
        {
            if (m_boundHandle != 0)
            {
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                m_boundHandle = 0;
            }
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