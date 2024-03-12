#pragma once

#include <optional>

#include "ECS/IManager.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public IComponentManager
        {
        public:
            ComponentManager(uint32_t maxComponents) :
                m_maxComponents(maxComponents)
            {
                m_buffer = malloc(sizeof(R) * m_maxComponents);
                m_components.resize(m_maxComponents, ComponentChunk());

                Entity::RegisterComponentManager(this);
            }

            virtual ~ComponentManager()
            {
                Entity::DeregisterComponentManager(this);
            }

            constexpr size_t GetHashCode() const override
            {
                return typeid(R).hash_code();
            }

            void EarlyUpdate()
            {
                for (auto& [available, component] : m_components)
                {
                    if constexpr (requires { component->EarlyUpdate(); })
                    {
                        if (component && component->IsEnabled())
                        {
                            component->EarlyUpdate();
                        }
                    }
                }
            }

            void Update(float deltaTime)
            {
                for (auto& [available, component] : m_components)
                {
                    if constexpr (requires { component->Update(deltaTime); })
                    {
                        if (component && component->IsEnabled())
                        {
                            component->Update(deltaTime);
                        }
                    }
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto& [available, component] : m_components)
                {
                    if constexpr (requires { component->FixedUpdate(fixedTime); })
                    {
                        if (component && component->IsEnabled())
                        {
                            component->FixedUpdate(fixedTime);
                        }
                    }
                }
            }

            void LateUpdate()
            {
                for (auto& [available, component] : m_components)
                {
                    if constexpr (requires { component->LateUpdate(); })
                    {
                        if (component && component->IsEnabled())
                        {
                            component->LateUpdate();
                        }
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

            R* Get(Entity& key) const
            {
                auto it = std::find_if(std::begin(m_components), std::end(m_components), [&key](const ComponentChunk& componentIndex)
                {
                    return componentIndex.Component && componentIndex.Component->GetEntity() == key;
                });
                return it != std::end(m_components) ? it->Component : nullptr;
            }

            void SetEnabled(Entity& key, bool enabled) override
            {
                auto it = std::find_if(std::begin(m_components), std::end(m_components), [&key](const ComponentChunk& componentIndex)
                {
                    return componentIndex.Component && componentIndex.Component->GetEntity() == key;
                });
                if (it != std::end(m_components))
                {
                    it->Component->SetEnabled(enabled);
                }
            }

        private:
            void* InsertV(Entity& key) override
            {
                return Insert(key);
            }

            void* GetV(Entity& key) const override
            {
                return Get(key);
            }

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

            void Remove(Entity& key) override
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

            uint32_t m_maxComponents;
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