#pragma once

#include "Entity.h"
#include "Interface/ITickable.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class Scene : Interface::NonCopyable
        {
        public:
            Scene();

            void SetRootEntity(Entity* entity)
            {
                m_rootEntity = entity;
            }

            void RegisterTickable(Interface::ITickable* tickable)
            {
                m_tickables.push_back(tickable);
            }

            void UnregisterTickable(Interface::ITickable* tickable)
            {
                m_tickables.erase(std::find(std::begin(m_tickables), std::end(m_tickables), tickable));
            }

            Entity* m_rootEntity = nullptr;
            std::vector<Interface::ITickable*> m_tickables;
        };
    }
}
