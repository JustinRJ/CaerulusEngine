#pragma once

#include <optional>

#include "ECS/IManager.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        constexpr uint32_t MaxComponents = 1000;

        template <class R>
        class ComponentManager : public IManager
        {
        public:
            ComponentManager()
            {
                StaticAssertComponentIsBase<R>();

                m_buffer = malloc(sizeof(R) * MaxComponents);
                m_components.resize(MaxComponents, ComponentChunk());

                ComponentManagerData data;
                data.ManagerTypeHash = GetManagedTypeHash();
                data.AddComponent = [&](Entity& key) { return Insert(key); };
                data.RemoveComponent = [&](Entity& key) { Remove(key); };
                Entity::RegisterComponentManager(data);
            }

            virtual ~ComponentManager() = default;

            constexpr size_t GetManagedTypeHash() const override
            {
                return typeid(R).hash_code();
            }

            void EarlyUpdate()
            {
                for (auto& [available, component] : m_components)
                {
                    if (component && component->IsEnabled())
                    {
                        component->EarlyUpdate();
                    }
                }
            }

            void Update(float deltaTime)
            {
                for (auto& [available, component] : m_components)
                {
                    if (component && component->IsEnabled())
                    {
                        component->Update(deltaTime);
                    }
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto& [available, component] : m_components)
                {
                    if (component && component->IsEnabled())
                    {
                        component->FixedUpdate(fixedTime);
                    }
                }
            }

            void LateUpdate()
            {
                for (auto& [available, component] : m_components)
                {
                    if (component && component->IsEnabled())
                    {
                        component->LateUpdate();
                    }
                }
            }

            void Reset()
            {
                for (auto& [available, component] : m_components)
                {
                    component->Reset();
                }
            }

        private:
            R* Insert(Entity& key)
            {
                for (uint32_t i = 0; i < m_components.size(); ++i)
                {
                    if (m_components[i].Available)
                    {
                        m_components[i].Available = false;

                        void* chunk = static_cast<uint8_t*>(m_buffer) + (i * sizeof(R));
                        /// Because this is placement-new allocated, it's actually
                        /// on the stack and thus doesn't need the delete keyword
                        R* location = new(chunk) R(key);
                        m_components[i].Component = location;
                        return location;
                    }
                }

                return nullptr;
            }

            void Remove(Entity& key)
            {
                auto it = std::find_if(std::begin(m_components), std::end(m_components), [&key](const ComponentChunk& componentIndex)
                {
                    return componentIndex.Component && componentIndex.Component->GetEntity() == key;
                });

                if (it != std::end(m_components))
                {
                    it->Available = true;
                    it->Component = nullptr;
                }
            }

            void* m_buffer = nullptr;
            struct ComponentChunk
            {
                bool Available = true;
                R* Component = nullptr;
            };

            std::vector<ComponentChunk> m_components;
        };
    }
}