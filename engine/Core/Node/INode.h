#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class CAERULUS_CORE INode : Interface::NonCopyable
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