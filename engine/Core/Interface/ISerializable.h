#pragma once

#define CAERULUS_CORE __declspec(dllexport)

namespace Core
{
    namespace Interface
    {
        class ISerializable
        {
        public:

            CAERULUS_CORE virtual void Serialize() = 0;

            CAERULUS_CORE virtual void Deserialize() = 0;
        };
    }
}