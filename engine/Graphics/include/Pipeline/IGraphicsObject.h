#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS IGraphicsObject : Core::Interface::NonCopyable
        {
        public:
            virtual ~IGraphicsObject() = default;

            virtual bool Init() = 0;
            virtual void Destroy() = 0;

            virtual void Bind() const = 0;
            virtual void Unbind() = 0;

            virtual unsigned int GetHandle() const = 0;
        };
    }
}