#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class IResetable
        {
        public:

            CAERULUS_CORE virtual void Reset() = 0;
        };
    }
}