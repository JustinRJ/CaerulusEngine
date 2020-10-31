#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class CAERULUS_CORE ISerializable
        {
        public:
            virtual void Serialize() = 0;
            virtual void Deserialize() = 0;
        };
    }
}