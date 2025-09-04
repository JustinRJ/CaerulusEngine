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

            Scene() :
                m_em(std::make_unique<EntityManager>()),
                m_rootEntity(&m_em->CreateEntity())
            {}

            ~Scene()
            {
                m_em->DestroyEntity(m_rootEntity->GetID());
            }

            Entity* GetRootEntity() const
            {
                return m_rootEntity;
            }

            EntityManager* GetEntityManager() const
            {
                return m_em.get();
            }

            void RegisterTickable(std::unique_ptr<Interface::ITickable>&& tickable)
            {
                m_tickables.insert({ tickable->GetHashCode(), std::move(tickable) });
            }

            template <typename T>
            void UnregisterTickable(Interface::ITickable* tickable)
            {
                auto it = std::find(std::begin(m_tickables), std::end(m_tickables), typeid(T).hash_code());
                if (it != std::end(m_tickables))
                {
                    m_tickables.erase(it);
                }
            }

            template <typename T>
            T* GetTickableOfType() const
            {
                size_t hashToFind = typeid(T).hash_code();
                auto it = m_tickables.find(hashToFind);
                if (it != std::end(m_tickables))
                {
                    return static_cast<T*>(it->second.get());
                }
                return nullptr;
            }

            std::vector<Interface::ITickable*> GetTickables() const
            {
                std::vector<Interface::ITickable*> tickables;
                for (const auto& [hash, tickable] : m_tickables)
                {
                    tickables.push_back(tickable.get());
                }
                return tickables;
            }

            std::unique_ptr<EntityManager> m_em;
            Entity* m_rootEntity = nullptr;
            std::unordered_map<size_t, std::unique_ptr<Interface::ITickable>> m_tickables;
        };
    }
}
