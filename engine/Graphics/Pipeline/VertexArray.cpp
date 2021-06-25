#include "stdafx.h"

#include "VertexArray.h"

namespace Graphics
{
    namespace Pipeline
    {
        unsigned int VertexArray::m_boundHandle = 0;

        VertexArray::VertexArray()
        {
            glGenVertexArrays(1, &m_handle);
        }

        VertexArray::~VertexArray()
        {
            glDeleteVertexArrays(1, &m_handle);
        }

        void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
        {
            Bind();
            vb.Bind();
            size_t offset = 0;
            const std::vector<VertexBufferElement>& elements = layout.GetElements();

            for (unsigned int i = 0; i < elements.size(); ++i)
            {
                const VertexBufferElement& element = elements[i];
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)offset);
                offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
            }
        }

        void VertexArray::Bind() const
        {
            if (m_boundHandle != m_handle)
            {
                glBindVertexArray(m_handle);
                m_boundHandle = m_handle;
            }
        }

        void VertexArray::Unbind()
        {
            if (m_boundHandle != 0)
            {
                glBindVertexArray(0);
                m_boundHandle = 0;
            }
        }

        GLuint VertexArray::GetHandle() const
        {
            return m_handle;
        }
    }
}