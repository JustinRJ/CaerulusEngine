#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ISerializable : private Interface::NonCopyable
        {
        public:
            virtual ~ISerializable() = default;

            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;
        };
    }
}