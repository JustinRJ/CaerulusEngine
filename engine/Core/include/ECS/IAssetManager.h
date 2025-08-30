#pragma once

#include "ECS/IHashable.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class CAERULUS_CORE IAssetManager : public IHashable, Interface::NonCopyable
        {
        public:
            virtual ~IAssetManager() = default;
        };
    }
}