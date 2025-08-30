#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>
#include <memory>
#include <algorithm>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"
#include "ECS/IComponentManager.h"

namespace Core
{
    namespace ECS
    {
        class Entity;

        class EntityManager
        {
        public:
            friend class Entity;

            Entity& CreateEntity()
            {
                uint32_t id = NextID();
                auto entity = std::make_unique<Entity>(*this, id);
                Entity& ref = *entity;
                m_entities[id] = std::move(entity);
                return ref;
            }

            Entity& CreateChild(Entity& parent)
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
                    m_entities.erase(it);  // unique_ptr cleanup
                    m_reusableEntityIDs.push_back(id);
                }
            }

            template <typename T>
            IComponentManager* GetComponentManager() const
            {
                auto it = m_componentManagerMap.find(typeid(T).hash_code());
                return it != m_componentManagerMap.end() ? it->second : nullptr;
            }

            template <typename T>
            void RegisterComponentManager(IComponentManager* mgr)
            {
                m_componentManagerMap[typeid(T).hash_code()] = mgr;
            }

            template <typename T>
            void DeregisterComponentManager()
            {
                m_componentManagerMap.erase(typeid(T).hash_code());
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

            uint32_t m_numEntities = 0;
            std::list<uint32_t> m_reusableEntityIDs;
            std::unordered_map<uint32_t, std::unique_ptr<Entity>> m_entities;
            std::unordered_map<size_t, IComponentManager*> m_componentManagerMap;
        };

        typedef std::bitset<32> EntityBitset;
        class CAERULUS_CORE Entity final : Interface::NonCopyable
        {
        public:
            // Root entity
            Entity(EntityManager& em, uint32_t id) :
                m_em(em), m_id(id), m_isEnabled(true)
            {}

            // Child entity, attaches itself to parent
            Entity(EntityManager& em, uint32_t id, Entity& parent) :
                m_em(em), m_id(id), m_parent(&parent),
                m_isEnabled(parent.IsEnabled())
            {
                m_parent->m_children.push_back(this);
            }

            ~Entity()
            {
                m_isDeleted = true;

                // Detach from parent
                if (m_parent && !m_parent->m_isDeleted)
                {
                    auto& siblings = m_parent->m_children;
                    siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
                }

                // Remove components
                for (auto& [hash, cm] : m_em.m_componentManagerMap)
                {
                    cm->Remove(*this);
                }

                // Destroy children (EntityManager owns them)
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
                    pChildren.erase(std::find_if(pChildren.begin(), pChildren.end(), [&](Entity* e)
                        {
                            return e->GetID() == GetID();
                        }));
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
                    m_em.DestroyEntity(e.GetID());
                    m_children.erase(it);
                }
            }

            void Remove()
            {
                m_em.DestroyEntity(m_id);
            }

            std::string_view GetName() const
            {
                return m_name;
            }

            void SetName(std::string_view name)
            {
                m_name = name;
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
                    for (auto& [hash, cm] : m_em.m_componentManagerMap)
                    {
                        cm->SetEnabled(*this, enabled);
                    }
                }

                for (Entity*& child : m_children)
                {
                    child->SetEnabled(enabled);
                }
            }

            EntityBitset& GetLayers()
            {
                return m_layers;
            }

            const EntityBitset& GetLayers() const
            {
                return m_layers;
            }

            std::vector<std::string>& GetTags()
            {
                return m_tags;
            }

            const std::vector<std::string>& GetTags() const
            {
                return m_tags;
            }

            Math::Transform& GetLocalTransform()
            {
                return m_localTransform;
            }

            const Math::Transform& GetLocalTransform() const
            {
                return m_localTransform;
            }

            Math::Transform GetWorldTransform() const
            {
                return m_parent ? m_parent->GetWorldTransform() * m_localTransform : m_localTransform;
            }

            void SetWorldTransform(const Math::Transform& transform)
            {
                m_localTransform = GetWorldTransform() * transform;
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
                    return nullptr;
                return static_cast<T*>(mgr->InsertV(*this));
            }

            template<class T>
            T* GetComponentOfType() const
            {
                auto* mgr = m_em.GetComponentManager<T>();
                if (!mgr)
                    return nullptr;
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
                    mgr->Remove(*this);
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
            std::string m_name;
            EntityBitset m_layers;
            Math::Transform m_localTransform;
            std::vector<std::string> m_tags;

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
    }
}
