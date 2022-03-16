#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ITickable : Interface::NonCopyable
        {
        public:
            virtual ~ITickable() = default;

            virtual void EarlyUpdate() {};
            virtual void Update(float deltaTime) {};
            virtual void FixedUpdate(float fixedTime) {};
            virtual void LateUpdate() {};

            virtual void Reset() {};
        };
    }
}