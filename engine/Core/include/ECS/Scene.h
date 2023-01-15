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

            void SetRootEntity(const std::shared_ptr<Entity>& entity)
            {
                m_rootEntity = entity;
            }

            std::shared_ptr<Entity> GetRootEntity() const
            {
                return m_rootEntity;
            }

            void RegisterTickable(std::shared_ptr<Interface::ITickable> tickable)
            {
                m_tickables.push_back(tickable);
            }

            void UnregisterTickable(std::shared_ptr<Interface::ITickable> tickable)
            {
                m_tickables.erase(std::find(std::begin(m_tickables), std::end(m_tickables), tickable));
            }

            template <typename T>
            std::shared_ptr<T> GetTickableOfType() const
            {
                for (auto& interface : m_tickables)
                {
                    if (auto t = dynamic_cast<T*>(interface.get()))
                    {
                        return std::static_pointer_cast<T>(interface);
                    }
                }
                return nullptr;
            }

            const std::vector<std::shared_ptr<Interface::ITickable>>& GetTickables() const
            {
                return m_tickables;
            }

            std::shared_ptr<Entity> m_rootEntity = nullptr;
            std::vector<std::shared_ptr<Interface::ITickable>> m_tickables;
        };
    }
}
