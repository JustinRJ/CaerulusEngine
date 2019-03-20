#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ITickable
        {
        public:

            virtual void PreUpdate() = 0;

            virtual void Update(float deltaTime) = 0;

            virtual void FixedUpdate(float fixedTime) = 0;

            virtual void PostUpdate() = 0;

            virtual void Reset() = 0;
        };
    }
}