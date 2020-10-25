#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS IBindable : public Core::Interface::NonCopyable
        {
        public:
            virtual void Bind() const = 0;

            virtual void Unbind() const = 0;
        };
    }
}