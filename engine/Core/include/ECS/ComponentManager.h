#pragma once

#include "ECS/IManager.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public IManager
        {
        public:
            ComponentManager() :
                m_typeHash(typeid(R).hash_code())
            {
                StaticAssertComponentIsBase<R>();

                ComponentManagerData data;
                data.ManagerTypeHash = GetManagedTypeHash();
                data.AddComponent = [&](Entity& key)
                {
                    const auto& newComponent = std::make_shared<R>(key);
                    Insert(key, newComponent);
                    return newComponent;
                };
                data.RemoveComponent = [&](Entity& key)
                {
                    Remove(key);
                };

                Entity::RegisterComponentManager(data);
            }

            virtual ~ComponentManager() = default;

            size_t GetManagedTypeHash() const override
            {
                return m_typeHash;
            }

            void EarlyUpdate()
            {
                for (auto& [entity, component] : m_components)
                {
                    if (component->IsEnabled())
                    {
                        component->EarlyUpdate();
                    }
                }
            }

            void Update(float deltaTime)
            {
                for (auto& [entity, component] : m_components)
                {
                    if (component->IsEnabled())
                    {
                        component->Update(deltaTime);
                    }
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto& [entity, component] : m_components)
                {
                    if (component->IsEnabled())
                    {
                        component->FixedUpdate(fixedTime);
                    }
                }
            }

            void LateUpdate()
            {
                for (auto& [entity, component] : m_components)
                {
                    if (component->IsEnabled())
                    {
                        component->LateUpdate();
                    }
                }
            }

            void Reset()
            {
                for (auto& [entity, component] : m_components)
                {
                    component->Reset();
                }
            }

        private:
            void Insert(Entity& key, const std::shared_ptr<R>& value)
            {
                if (value)
                {
                    m_components.insert({ &key, value });
                }
            }

            void Remove(Entity& key)
            {
                auto it = m_components.find(&key);
                if (it != std::end(m_components))
                {
                    m_components.erase(it);
                }
            }

            size_t m_typeHash;
            std::map<Entity*, std::shared_ptr<R>> m_components;
        };
    }
}