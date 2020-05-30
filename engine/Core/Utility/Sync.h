#pragma once

#include "Core/Interface/NonCopyable.h"
#include <future>

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE Sync : public Interface::NonCopyable
        {
        public:
            Sync() = delete;
            virtual ~Sync() = default;


        private:
        };
    }
}
