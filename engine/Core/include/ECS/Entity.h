#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"
#include "ECS/ThreadSafe.h"

namespace Core
{
    namespace ECS
    {
        class Entity;
        class Component;

        struct ComponentData
        {
            Component* Component;
            std::function<void(const Entity&)> OnDestroy;
        };

        typedef std::bitset<32> EntityBitset;

        class CAERULUS_CORE Entity : Interface::NonCopyable
        {
        public:
            Entity(Entity* parent);
            ~Entity();

            unsigned int GetID() const;

            unsigned int GetEntityCount() const;

            Entity* GetParent() const;
            const std::vector<Entity*>& GetChildren() const;

            const std::string& GetName() const;
            void SetName(const std::string& layer);

            bool GetEnabled() const;
            void SetEnabled(bool enabled);

            const EntityBitset& GetLayers() const;
            void SetLayers(const EntityBitset& layer);

            const std::vector<std::string>& GetTags() const;
            void SetTags(const std::vector<std::string>& tags);

            Math::Transform GetWorldTransform() const;
            void SetWorldTransform(const Math::Transform& transform);

            const Math::Transform& GetLocalTransform() const;
            void SetLocalTransform(const Math::Transform& transform);

            void SwapID(Entity& e);

            void AddComponent(const ComponentData& componentData);
            void RemoveComponent(const Component& component);

            template<class T>
#ifdef THREAD_SAFE
            const T* GetComponentOfType() const
#endif
#ifdef THREAD_UNSAFE
            T* GetComponentOfType() const
#endif
            {
                T* foundType = nullptr;
                auto it = std::begin(m_components);
                while (!foundType && it != std::end(m_components))
                {
                    if (auto type = dynamic_cast<T*>(it->Component))
                    {
                        foundType = type;
                    }
                    it++;
                }
                return foundType;
            }

            template<class T>
#ifdef THREAD_SAFE
            std::vector<const T*> GetComponentsOfType() const
            {
                std::vector<const T*> foundTypes;
#endif
#ifdef THREAD_UNSAFE
                std::vector<T*> GetComponentsOfType() const
                {
                    std::vector<T*> foundTypes;
#endif
                foundTypes.push_back(GetComponentOfType<T>());
                for (Entity* child : m_children)
                {
                    auto childFoundTypes = child->GetComponentsOfType<T>();
                    foundTypes.insert(std::end(foundTypes), std::begin(childFoundTypes), std::end(childFoundTypes));
                }
                return foundTypes;
            }

            template<class T>
            void RemoveComponentOfType()
            {
                if (auto component = GetComponentOfType<T>())
                {
                    RemoveComponent(*component);
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
            bool m_enabled;
            unsigned int m_ID;
            std::string m_name;
            EntityBitset m_layers;
            std::vector<std::string> m_tags;

            Entity* m_parent;
            std::vector<Entity*> m_children;

            Math::Transform m_localTransform;
            std::vector<ComponentData> m_components;

            static unsigned int s_numEntities;
            static unsigned int s_maxDiscardedEntityIDs;
            static std::list<unsigned int> s_reusableEntityIDs;
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
