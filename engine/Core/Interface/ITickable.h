#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class ITickable
        {
        public:

            CAERULUS_CORE virtual void PreUpdate() = 0;

            CAERULUS_CORE virtual void Update(float deltaTime) = 0;

            CAERULUS_CORE virtual void FixedUpdate(float fixedTime) = 0;

            CAERULUS_CORE virtual void PostUpdate() = 0;

            CAERULUS_CORE virtual void Reset() = 0;
        };
    }
}