#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ITickable : private Interface::NonCopyable
        {
        public:
            virtual ~ITickable() = default;

            virtual void PreUpdate(float deltaTime) = 0;
            virtual void Update(float deltaTime) = 0;
            virtual void FixedUpdate(float fixedTime) = 0;
            virtual void Reset() = 0;
        };
    }
}