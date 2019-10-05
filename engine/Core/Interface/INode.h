#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE INode
        {
        public:

            virtual unsigned int GetID() const = 0;
            virtual unsigned int GetNodeCount() const = 0;

            virtual void SwapID(INode& e) = 0;

        protected:
            virtual void SetID(unsigned int id) = 0;
        };
    }
}