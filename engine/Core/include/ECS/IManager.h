#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class CAERULUS_CORE IManager : Interface::NonCopyable
        {
        public:
            IManager() = default;
            virtual ~IManager() = default;

            virtual size_t GetManagedTypeHash() const = 0;
        };
    }
}