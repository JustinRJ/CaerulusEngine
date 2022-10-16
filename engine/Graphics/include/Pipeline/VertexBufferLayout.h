#pragma once

#include "Log/Log.h"

#include <vector>

namespace Graphics
{
    namespace Pipeline
    {
        struct VertexBufferElement
        {
            uint32_t Type;
            uint32_t Count;
            unsigned char Normalized;

            static uint32_t GetSizeOfType(uint32_t type)
            {
                switch (type)
                {
#ifdef _x64
                    case GL_UNSIGNED_INT:   return 8;
#endif
                    case GL_FLOAT:          return 4;
                    case GL_UNSIGNED_INT:   return 4;
                    case GL_UNSIGNED_BYTE:  return 1;
                }
                Core::Log::LogError("VertexBufferlayout::GetSizeOfType - unsupported type");
                exit(1);
            }
        };

        class VertexBufferLayout
        {
        public:
            VertexBufferLayout() = default;

            template <typename T>
            void Push(uint32_t count)
            {
                Core::Log::LogError("VertexBufferlayout::Push - unsupported type");
                exit(1);
            }

            template <>
            void Push<float>(uint32_t count)
            {
                m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
            }

            template <>
            void Push<uint32_t>(uint32_t count)
            {
                m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
            }

            template <>
            void Push<unsigned char>(uint32_t count)
            {
                m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
                m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
            }

            uint32_t GetStride() const
            {
                return m_stride;
            }

            const std::vector<VertexBufferElement>& GetElements() const
            {
                return m_elements;
            }

        private:
            uint32_t m_stride = 0;
            std::vector<VertexBufferElement> m_elements;
        };
    }
}