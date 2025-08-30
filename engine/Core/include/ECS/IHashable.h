#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace ECS
    {
        class CAERULUS_CORE IHashable
        {
        public:
            virtual ~IHashable() = default;
            virtual size_t GetHashCode() const = 0;
        };
    }
}