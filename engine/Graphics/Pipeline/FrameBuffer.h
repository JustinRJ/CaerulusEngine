#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS FrameBuffer : public Core::Interface::NonCopyable
        {
        public:
            FrameBuffer()
            {
            }

            ~FrameBuffer()
            {
            }

            void Bind() const;
            static void Unbind();

        private:
            GLuint m_handle;
        };
    }
}