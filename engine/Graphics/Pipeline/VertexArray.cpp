#include "stdafx.h"

#include "Pipeline\VertexArray.h"

namespace Graphics
{
    namespace Pipeline
    {
        VertexArray::VertexArray()
        {
            glGenVertexArrays(1, &m_handle);
        }

        VertexArray::~VertexArray()
        {
            glDeleteVertexArrays(1, &m_handle);
        }

        void VertexArray::AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout)
        {
            Bind();
            vb.Bind();
            size_t offset = 0;
            const std::vector<VertexBufferElement>& elements = layout.GetElements();

            for (uint32_t i = 0; i < elements.size(); ++i)
            {
                const VertexBufferElement& element = elements[i];
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (const void*)offset);
                offset += element.Count * VertexBufferElement::GetSizeOfType(element.Type);
            }
        }

        void VertexArray::Bind()
        {
            glBindVertexArray(m_handle);
        }

        void VertexArray::Unbind()
        {
            glBindVertexArray(0);
        }

        GLuint VertexArray::GetHandle() const
        {
            return m_handle;
        }
    }
}