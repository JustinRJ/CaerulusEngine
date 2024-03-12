#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class CAERULUS_CORE IHashable : Interface::NonCopyable
        {
        public:
            virtual ~IHashable() = default;
            virtual size_t GetHashCode() const = 0;
        };

        class Entity;
        class CAERULUS_CORE IComponentManager : public IHashable
        {
        public:
            virtual ~IComponentManager() = default;

            virtual void SetEnabled(Entity& key, bool enabled) = 0;
            virtual void Remove(Entity& key) = 0;

        private:
            friend Entity;
            virtual void* InsertV(Entity& key) = 0;
            virtual void* GetV(Entity& key) const = 0;
        };
    }
}