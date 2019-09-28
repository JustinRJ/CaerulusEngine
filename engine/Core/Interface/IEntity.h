#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE IEntity
        {
        public:

            virtual unsigned int GetID() const = 0;
            virtual unsigned int GetEntityCount() const = 0;

            virtual void SwapID(IEntity& e) = 0;

        protected:
            virtual void SetID(unsigned int id) = 0;
        };
    }
}