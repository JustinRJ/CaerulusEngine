#pragma once

#include "Core/Interface/NonCopyable.h"
#include "Core/Math/Transform.h"

namespace Core
{
    namespace Node
    {
        class INode : Interface::NonCopyable
        {
        public:
            virtual unsigned int GetID() const = 0;
            virtual unsigned int GetNodeCount() const = 0;
            virtual const Math::Transform& GetTransform() const = 0;

            virtual void SwapID(INode& e) = 0;

        protected:
            virtual void SetID(unsigned int id) = 0;
        };
    }
}