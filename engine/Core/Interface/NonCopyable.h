#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE NonCopyable
        {
        protected:
            NonCopyable() = default;
            ~NonCopyable() = default;

            NonCopyable(const NonCopyable&) = delete;
            NonCopyable& operator=(const NonCopyable&) = delete;
        };
    }
}