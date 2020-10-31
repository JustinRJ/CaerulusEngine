#pragma once

#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Pipeline
    {
        struct VertexBufferElement
        {
            unsigned int Type;
            unsigned int Count;
            unsigned char Normalized;

            static unsigned int GetSizeOfType(unsigned int type)
            {
                switch (type)
                {
                    case GL_FLOAT:          return 4;
                    case GL_UNSIGNED_INT:   return 4;
                    case GL_UNSIGNED_BYTE:  return 1;
#ifdef _x64
                    case GL_UNSIGNED_INT:   return 8;
#endif
                }
                Core::Logging::Log::LogError("VertexBufferlayout::GetSizeOfType - unsupported type");
                exit(1);
            }
        };

        class VertexBufferLayout
        {
        public:
            VertexBufferLayout() = default;
            ~VertexBufferLayout() = default;

            template <typename T>
            void Push(unsigned int count)
            {
                Core::Logging::Log::LogError("VertexBufferlayout::Push - unsupported type");
                exit(1);
            }

            template <>
            void Push<float>(unsigned int count)
            {
                m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
            }

            template <>
            void Push<unsigned int>(unsigned int count)
            {
                m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
            }

            template <>
            void Push<unsigned char>(unsigned int count)
            {
                m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
            }

            unsigned int GetStride() const
            {
                return m_stride;
            }

            const std::vector<VertexBufferElement>& GetElements() const
            {
                return m_elements;
            }

        private:
            unsigned int m_stride = 0;
            std::vector<VertexBufferElement> m_elements;
        };
    }
}