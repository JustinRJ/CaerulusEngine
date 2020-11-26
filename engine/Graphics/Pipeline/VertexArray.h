#pragma once

#include "Core/Interface/NonCopyable.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Graphics
{
    namespace Pipeline
    {
        class VertexArray : public Core::Interface::NonCopyable
        {
        public:
            VertexArray();
            ~VertexArray();

            void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

            void Bind() const;
            void Unbind() const;

            GLuint GetHandle() const;

        private:
            GLuint m_handle;
            static GLuint m_boundHandle;
        };
    }
}