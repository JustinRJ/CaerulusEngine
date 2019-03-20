#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE IResetable
        {
        public:

            virtual void Reset() = 0;
        };
    }
}