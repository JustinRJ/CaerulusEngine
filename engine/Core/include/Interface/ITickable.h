#pragma once

#include "Interface/IHashable.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ITickable : public IHashable, NonCopyable
        {
        public:
            virtual ~ITickable() = default;

            virtual void EarlyTick() {};
            virtual void Tick(float deltaTime) {};
            virtual void FixedTick(float fixedTime) {};
            virtual void LateTick() {};

            virtual void Reset() {};
        };
    }
}