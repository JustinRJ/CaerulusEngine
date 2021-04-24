#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE NonMovable
        {
        protected:
            NonMovable() = default;
            ~NonMovable() = default;

            NonMovable(NonMovable&&) = delete;
            NonMovable& operator=(NonMovable&&) = delete;
        };
    }
}