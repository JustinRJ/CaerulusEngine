#pragma once

#include "ECS/IHashable.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class Entity;
        class CAERULUS_CORE IComponentManager : public IHashable, Interface::NonCopyable
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