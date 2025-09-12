#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>
#include <memory>
#include <algorithm>

#include "Math/Transform.h"
#include "Interface/ITickable.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class Entity;
        template<class T>
        class ComponentManager;

        class IComponentManager : public Interface::ITickable
        {
        public:
            virtual ~IComponentManager() = default;

            virtual void SetEnabled(const Entity& key, bool enabled) = 0;
            virtual void Remove(const Entity& key) = 0;
        };

        class EntityManager : public Interface::ITickable
        {
        public:
            friend class Entity;

            size_t GetHashCode() const override
            {
                return typeid(EntityManager).hash_code();
            }

            Entity& CreateEntity()
            {
                uint32_t id = NextID();
                auto entity = std::make_unique<Entity>(*this, id);
                Entity& ref = *entity;
                m_entities[id] = std::move(entity);
                return ref;
            }

            Entity& CreateEntity(Entity& parent)
            {
                uint32_t id = NextID();
                auto entity = std::make_unique<Entity>(*this, id, parent);
                Entity& ref = *entity;
                m_entities[id] = std::move(entity);
                return ref;
            }

            void DestroyEntity(uint32_t id)
            {
                auto it = m_entities.find(id);
                if (it != m_entities.end())
                {
                    m_entities.erase(it);
                    m_reusableEntityIDs.push_back(id);
                }
            }

            template<class T>
            ComponentManager<T>* GetComponentManager()
            {
                size_t hash = typeid(T).hash_code();
                auto it = m_componentManagersMap.find(hash);
                if (it != m_componentManagersMap.end())
                {
                    return static_cast<ComponentManager<T>*>(it->second.get());
                }

                auto newManager = std::make_unique<ComponentManager<T>>();
                auto manager = newManager.get();
                m_componentManagersVec.push_back(manager);
                m_componentManagersMap[hash] = std::move(newManager);
                return manager;
            }

        private:
            uint32_t NextID()
            {
                if (!m_reusableEntityIDs.empty())
                {
                    auto id = m_reusableEntityIDs.front();
                    m_reusableEntityIDs.pop_front();
                    return id;
                }
                return m_numEntities++;
            }

            void Remove(Entity& e)
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->Remove(e);
                }
            }

            void SetEnabled(Entity& e, bool enabled)
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->SetEnabled(e, enabled);
                }
            }

            void EarlyTick() override
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->EarlyTick();
                }
            }

            void Tick(float deltaTime) override
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->Tick(deltaTime);
                }
            }

            void FixedTick(float fixedTime) override
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->FixedTick(fixedTime);
                }
            }

            void LateTick() override
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->LateTick();
                }
            }

            void Reset() override
            {
                for (auto cm : m_componentManagersVec)
                {
                    cm->Reset();
                }
            }

            uint32_t m_numEntities = 0;
            std::list<uint32_t> m_reusableEntityIDs;
            std::unordered_map<uint32_t, std::unique_ptr<Entity>> m_entities;

            std::vector<IComponentManager*> m_componentManagersVec;
            std::unordered_map<size_t, std::unique_ptr<IComponentManager>> m_componentManagersMap;
        };

        class CAERULUS_CORE Entity final : Interface::NonCopyable
        {
        public:
            Entity(EntityManager& em, uint32_t id) :
                m_em(em), m_id(id), m_isEnabled(true)
            {}

            Entity(EntityManager& em, uint32_t id, Entity& parent) :
                m_em(em), m_id(id), m_parent(&parent),
                m_isEnabled(parent.IsEnabled())
            {
                m_parent->m_children.push_back(this);
            }

            ~Entity()
            {
                m_isDeleted = true;

                if (m_parent && !m_parent->m_isDeleted)
                {
                    auto& siblings = m_parent->m_children;
                    siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
                }

                m_em.Remove(*this);

                for (Entity* child : m_children)
                {
                    m_em.DestroyEntity(child->GetID());
                }
                m_children.clear();
            }

            uint32_t GetID() const
            {
                return m_id;
            }

            Entity* GetParent() const
            {
                return m_parent;
            }

            void SetParent(Entity* parent)
            {
                if (m_parent)
                {
                    auto& pChildren = m_parent->m_children;
                    pChildren.erase(std::find_if(pChildren.begin(), pChildren.end(),
                        [this](Entity* e) { return e->GetID() == GetID(); }));
                }
                m_parent = parent;

                if (m_parent)
                {
                    m_parent->m_children.push_back(this);
                    SetEnabled(m_parent->IsEnabled());
                }
            }

            const std::vector<Entity*>& GetChildren() const
            {
                return m_children;
            }

            void RemoveChild(Entity& e)
            {
                auto it = std::find(m_children.begin(), m_children.end(), &e);
                if (it != m_children.end())
                {
                    m_children.erase(it);
                    m_em.DestroyEntity(e.GetID());
                }
            }

            void Remove()
            {
                m_em.DestroyEntity(m_id);
            }

            bool IsEnabled() const
            {
                return m_isEnabled;
            }

            void SetEnabled(bool enabled)
            {
                if (m_isEnabled != enabled)
                {
                    m_isEnabled = enabled;
                    m_em.SetEnabled(*this, enabled);
                }

                for (Entity* child : m_children)
                {
                    child->SetEnabled(enabled);
                }
            }

            // TODO - re-create a transform component for entities, don't rely on GetComponent
            Math::Transform GetWorldTransform() const
            {
                if (auto local = GetComponentOfType<Math::Transform>())
                {
                    return m_parent ? m_parent->GetWorldTransform() * *local : *local;
                }
                return Math::Transform();
            }

            void SwapID(Entity& e)
            {
                uint32_t temp = m_id;
                m_id = e.m_id;
                e.m_id = temp;
            }

            std::vector<Entity*> ToVector() const
            {
                std::vector<Entity*> vec;
                vec.insert(vec.begin(), m_children.begin(), m_children.end());
                for (auto child : m_children)
                {
                    vec.insert(vec.begin(), child->m_children.begin(), child->m_children.end());
                }
                return vec;
            }

            template<class T>
            T* AddComponentOfType()
            {
                auto* mgr = m_em.GetComponentManager<T>();
                if (!mgr)
                {
                    return nullptr;
                }
                return static_cast<T*>(mgr->Insert(*this));
            }

            template<class T>
            T* GetComponentOfType() const
            {
                auto* mgr = m_em.GetComponentManager<T>();
                if (!mgr)
                {
                    return nullptr;
                }
                return mgr->Get(*this);
            }

            template<class T>
            std::vector<T*> AddComponentsOfType()
            {
                std::vector<T*> components;
                components.push_back(AddComponentOfType<T>());
                for (Entity* child : m_children)
                {
                    auto childComponents = child->AddComponentsOfType<T>();
                    components.insert(components.end(), childComponents.begin(), childComponents.end());
                }
                return components;
            }

            template<class T>
            std::vector<T*> GetComponentsOfType() const
            {
                std::vector<T*> components;
                components.push_back(GetComponentOfType<T>());
                for (Entity* child : m_children)
                {
                    auto childComponents = child->GetComponentsOfType<T>();
                    components.insert(components.end(), childComponents.begin(), childComponents.end());
                }
                return components;
            }

            template<class T>
            void RemoveComponentOfType()
            {
                auto* mgr = m_em.GetComponentManager<T>();
                if (mgr)
                {
                    mgr->Remove(*this);
                }
            }

            template<class T>
            void RemoveComponentsOfType()
            {
                RemoveComponentOfType<T>();
                for (Entity* child : m_children)
                {
                    child->RemoveComponentsOfType<T>();
                }
            }

        private:
            bool m_isDeleted = false;
            bool m_isEnabled = false;
            uint32_t m_id;
            EntityManager& m_em;
            Entity* m_parent = nullptr;
            std::vector<Entity*> m_children;
        };

        inline bool operator== (const Entity& left, const Entity& right)
        {
            return left.GetID() == right.GetID();
        }

        inline bool operator< (const Entity& left, const Entity& right)
        {
            return left.GetID() < right.GetID();
        }

        inline bool operator> (const Entity& left, const Entity& right)
        {
            return right < left;
        }

        inline bool operator<= (const Entity& left, const Entity& right)
        {
            return !(left > right);
        }

        inline bool operator>= (const Entity& left, const Entity& right)
        {
            return !(left < right);
        }

        template <class R>
        class ComponentManager : public IComponentManager
        {
        public:
            friend Entity;

            virtual ~ComponentManager() = default;

            constexpr size_t GetHashCode() const override
            {
                return typeid(R).hash_code();
            }

            void EarlyTick()
            {
                for (auto& entry : m_components)
                {
                    auto& comp = entry.Component;
                    if constexpr (requires { comp->EarlyTick(); })
                    {
                        if (comp && entry.Enabled)
                        {
                            comp->EarlyTick();
                        }
                    }
                }
            }

            void Tick(float deltaTime)
            {
                for (auto& entry : m_components)
                {
                    auto& comp = entry.Component;
                    if constexpr (requires { comp->Tick(deltaTime); })
                    {
                        if (comp && entry.Enabled)
                        {
                            comp->Tick(deltaTime);
                        }
                    }
                }
            }

            void FixedTick(float fixedTime)
            {
                for (auto& entry : m_components)
                {
                    auto& comp = entry.Component;
                    if constexpr (requires { comp->FixedTick(fixedTime); })
                    {
                        if (comp && entry.Enabled)
                        {
                            comp->FixedTick(fixedTime);
                        }
                    }
                }
            }

            void LateTick()
            {
                for (auto& entry : m_components)
                {
                    auto& comp = entry.Component;
                    if constexpr (requires { comp->LateTick(); })
                    {
                        if (comp && entry.Enabled)
                        {
                            comp->LateTick();
                        }
                    }
                }
            }

            void Reset()
            {
                for (auto& entry : m_components)
                {
                    auto& comp = entry.Component;
                    if constexpr (requires { comp->Reset(); })
                    {
                        if (comp)
                        {
                            comp->Reset();
                        }
                    }
                }
            }

            R* Get(const Entity& key) const
            {
                auto it = m_entityToIndex.find(&key);
                if (it != m_entityToIndex.end())
                {
                    return m_components[it->second].Component.get();
                }
                return nullptr;
            }

        private:

            struct ComponentEntry
            {
                bool Enabled = false;
                const Entity* Entity = nullptr;
                std::unique_ptr<R> Component = nullptr;
            };

            void SetEnabled(const Entity& key, bool enabled) override
            {
                auto it = m_entityToIndex.find(&key);
                if (it != m_entityToIndex.end())
                {
                    auto comp = Get(key);
                    bool& e = m_components[it->second].Enabled;

                    if (key.IsEnabled())
                    {
                        if (e != enabled)
                        {
                            e = enabled;
                            if (enabled)
                            {
                                if constexpr (requires { comp->OnEnable(); })
                                {
                                    comp->OnEnable();
                                }
                            }
                            else
                            {
                                if constexpr (requires { comp->OnDisable(); })
                                {
                                    comp->OnDisable();
                                }
                            }
                        }
                    }
                    else if (e != false)
                    {
                        e = false;
                        if constexpr (requires { comp->OnDisable(); })
                        {
                            comp->OnDisable();
                        }
                    }
                }
            }

            bool IsEnabled(const Entity& key, bool enabled) const
            {
                if (!key.IsEnabled())
                {
                    return false;
                }
                auto it = m_entityToIndex.find(&key);
                if (it != m_entityToIndex.end())
                {
                    return m_components[it->second].Enabled;
                }
                return false;
            }

            R* Insert(const Entity& key)
            {
                if (auto existing = Get(key))
                {
                    return existing;
                }

                auto comp = std::make_unique<R>();
                m_components.push_back(ComponentEntry{ key.IsEnabled(), &key, std::move(comp) });
                m_entityToIndex[&key] = m_components.size() - 1;
                return m_components.back().Component.get();
            }

            void Remove(const Entity& key) override
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
                    m_entityToIndex[m_components[i].Entity] = i;
                }
            }

            std::vector<ComponentEntry> m_components;
            std::unordered_map<const Entity*, size_t> m_entityToIndex;
        };
    }
}
