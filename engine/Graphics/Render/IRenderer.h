#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Render
    {
        class CAERULUS_GRAPHICS IRenderer : public Core::Interface::NonCopyable
        {
        public:
            virtual void FlushErrors() = 0;
            virtual void Clear() = 0;
            virtual void DrawAll(float deltaTime) = 0;
            virtual void SwapBuffer(float deltaTime) = 0;
        };
    }
}