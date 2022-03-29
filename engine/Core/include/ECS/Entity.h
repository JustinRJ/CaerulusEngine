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
            size_t ComponentTypeHash = 0;
            Component* Component = nullptr;
        };

        struct ComponentManagerData
        {
            size_t ManagedTypeHash = 0;
            std::function<Component*(Entity&)> AddComponent = {};
            std::function<void(Entity&)> RemoveComponent = {};
        };

        typedef std::bitset<32> EntityBitset;

        class CAERULUS_CORE Entity : Interface::NonCopyable
        {
        public:
            Entity(Entity* parent = nullptr);
            ~Entity();

            unsigned int GetID() const;

            Entity* GetParent() const;
            void SetParent(Entity* parent);

            const std::vector<Entity*>& GetChildren() const;

            const std::string& GetName() const;
            void SetName(const std::string& layer);

            bool IsEnabled() const;
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

            static unsigned int GetEntityCount();
            static void RegisterComponentManager(const ComponentManagerData& componentManager);

            template<class T>
#ifdef THREAD_SAFE
            const T* AddComponentOfType()
#else
            T* AddComponentOfType()
#endif
            {
                return static_cast<T*>(AddComponentOfTypeInner(typeid(T).hash_code()));
            }

            template<class T>
#ifdef THREAD_SAFE
            const T* GetComponentOfType() const
#else
            T* GetComponentOfType() const
#endif
            {
                return static_cast<T*>(GetComponentOfTypeInner(typeid(T).hash_code()));
            }

            // TODO - create std::vector<const T*> impl
#ifndef THREAD_SAFE
            template<class T>
            std::vector<T*> AddComponentsOfType()
            {
                return AddComponentsOfTypeInner(typeid(T).hash_code());
            }

            template<class T>
            std::vector<T*> GetComponentsOfType() const
            {
                return GetComponentsOfTypeInner(typeid(T).hash_code());
            }
#endif

            template<class T>
            void RemoveComponentOfType()
            {
                RemoveComponentOfTypeInner(typeid(T).hash_code());
            }

            template<class T>
            void RemoveComponentsOfType()
            {
                RemoveComponentsOfTypeInner(typeid(T).hash_code());
            }

        private:

            Component* AddComponentOfTypeInner(size_t typeToAdd);
            std::vector<Component*> AddComponentsOfTypeInner(size_t typeToAdd);

            Component* GetComponentOfTypeInner(size_t typeToFind) const;
            std::vector<Component*> GetComponentsOfTypeInner(size_t typeToFind) const;

            void RemoveComponentOfTypeInner(size_t typeToRemove);
            void RemoveComponentsOfTypeInner(size_t typeToRemove);

            bool m_enabled;
            unsigned int m_id;
            std::string m_name;
            EntityBitset m_layers;
            std::vector<std::string> m_tags;

            Entity* m_parent;
            std::vector<Entity*> m_children;

            Math::Transform m_localTransform;
            std::vector<ComponentData> m_components;

            static std::vector<ComponentManagerData> s_componentManagers;

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
