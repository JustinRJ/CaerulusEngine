#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>

#include "Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS IndexBuffer : Core::Interface::NonCopyable
        {
        public:
            IndexBuffer(const unsigned int* data, size_t count);
            ~IndexBuffer();

            void Bind();
            static void Unbind();

            GLuint GetHandle() const;
            size_t GetCount() const;

        private:
            GLuint m_handle;
            static GLuint m_boundHandle;
            size_t m_count;
        };
    }
}