#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

#include "ECS/Component.h"
#include "ECS/IComponentManager.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public IComponentManager
        {
        public:
            ComponentManager(EntityManager& em) :
                m_em(em)
            {
                m_em.RegisterComponentManager<R>(this);
            }

            virtual ~ComponentManager()
            {
                m_em.DeregisterComponentManager<R>();
            }

            constexpr size_t GetHashCode() const override
            {
                return typeid(R).hash_code();
            }

            void EarlyUpdate()
            {
                for (auto& comp : m_components)
                {
                    if constexpr (requires { comp->EarlyUpdate(); })
                    {
                        if (comp && comp->IsEnabled())
                        {
                            comp->EarlyUpdate();
                        }
                    }
                }
            }

            void Update(float deltaTime)
            {
                for (auto& comp : m_components)
                {
                    if constexpr (requires { comp->Update(deltaTime); })
                    {
                        if (comp && comp->IsEnabled())
                        {
                            comp->Update(deltaTime);
                        }
                    }
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto& comp : m_components)
                {
                    if constexpr (requires { comp->FixedUpdate(fixedTime); })
                    {
                        if (comp && comp->IsEnabled())
                        {
                            comp->FixedUpdate(fixedTime);
                        }
                    }
                }
            }

            void LateUpdate()
            {
                for (auto& comp : m_components)
                {
                    if constexpr (requires { comp->LateUpdate(); })
                    {
                        if (comp && comp->IsEnabled())
                        {
                            comp->LateUpdate();
                        }
                    }
                }
            }

            void Reset()
            {
                for (auto& comp : m_components)
                {
                    if (comp)
                    {
                        comp->Reset();
                    }
                }
            }

            R* Get(Entity& key) const
            {
                auto it = m_entityToIndex.find(&key);
                if (it != m_entityToIndex.end())
                    return m_components[it->second].get();
                return nullptr;
            }

            void SetEnabled(Entity& key, bool enabled) override
            {
                if (auto comp = Get(key))
                {
                    comp->SetEnabled(enabled);
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
                if (auto existing = Get(key))
                {
                    return existing;
                }

                auto comp = std::make_unique<R>(key);
                m_components.push_back(std::move(comp));
                m_entityToIndex[&key] = m_components.size() - 1;
                return m_components.back().get();
            }

            void Remove(Entity& key) override
            {
                auto it = m_entityToIndex.find(&key);
                if (it == m_entityToIndex.end())
                {
                    return;
                }

                size_t index = it->second;

                // erase from vector while preserving order
                m_components.erase(m_components.begin() + index);

                // remove from map
                m_entityToIndex.erase(it);

                // update indices for all elements after the removed one
                for (size_t i = index; i < m_components.size(); ++i)
                {
                    m_entityToIndex[&(m_components[i]->GetEntity())] = i;
                }
            }

            EntityManager& m_em;
            std::vector<std::unique_ptr<R>> m_components;
            std::unordered_map<Entity*, size_t> m_entityToIndex;
        };
    }
}
