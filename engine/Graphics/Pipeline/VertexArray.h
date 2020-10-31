#pragma once

#include "IBindable.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Graphics
{
    namespace Pipeline
    {
        class VertexArray : public IBindable
        {
        public:
            VertexArray()
            {
                glGenVertexArrays(1, &m_handle);
            }

            ~VertexArray()
            {
                glDeleteVertexArrays(1, &m_handle);
            }

            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
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

            void Bind() const override
            {
                glBindVertexArray(m_handle);
            }

            void Unbind() const override
            {
                glBindVertexArray(0);
            }

            unsigned int GetHandle() const override
            {
                return m_handle;
            }

        private:
            unsigned int m_handle;
        };
    }
}