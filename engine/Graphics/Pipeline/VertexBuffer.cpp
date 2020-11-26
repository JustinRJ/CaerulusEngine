#include "stdafx.h"

#include "VertexBuffer.h"

namespace Graphics
{
    namespace Pipeline
    {
        GLuint VertexBuffer::m_boundHandle;

        VertexBuffer::VertexBuffer(const void* data, size_t size) :
            m_size(size)
        {
            glGenBuffers(1, &m_handle);
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
            glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }

        VertexBuffer::~VertexBuffer()
        {
            glDeleteBuffers(1, &m_handle);
        }

        void VertexBuffer::Bind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_handle);
        }

        void VertexBuffer::Unbind() const
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        GLuint VertexBuffer::GetHandle() const
        {
            return m_handle;
        }

        size_t VertexBuffer::GetSize() const
        {
            return m_size;
        }
    }
}