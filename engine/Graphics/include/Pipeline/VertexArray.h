#pragma once

#include <glew.h>

#include "Interface/NonCopyable.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS VertexArray : Core::Interface::NonCopyable
        {
        public:
            VertexArray();
            ~VertexArray();

            void AddBuffer(VertexBuffer& vb, const VertexBufferLayout& layout);

            void Bind();
            static void Unbind();

            GLuint GetHandle() const;

        private:
            GLuint m_handle;
            static GLuint m_boundHandle;
        };
    }
}