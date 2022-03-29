#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>

#include "Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS VertexBuffer : Core::Interface::NonCopyable
        {
        public:
            VertexBuffer(const void* data, size_t size);
            ~VertexBuffer();

            void Bind();
            static void Unbind();

            GLuint GetHandle() const;
            size_t GetSize() const;

        private:
            GLuint m_handle;
            size_t m_size;
        };
    }
}