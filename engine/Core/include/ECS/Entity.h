#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>
#include <algorithm>

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
            std::weak_ptr<Component> Component;
        };

        struct ComponentManagerData
        {
            size_t ManagedTypeHash = 0;
            std::function<std::shared_ptr<Component>(Entity&)> AddComponent = {};
            std::function<void(Entity&)> RemoveComponent = {};
        };

        typedef std::bitset<32> EntityBitset;

        template<class T>
        inline void StaticAssertComponentIsBase()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a child of Component");
        }

        class CAERULUS_CORE Entity final : Interface::NonCopyable
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
            std::shared_ptr<T> AddComponentOfType()
            {
                StaticAssertComponentIsBase<T>();
                return std::static_pointer_cast<T>(AddComponentOfTypeInner(typeid(T).hash_code()));
            }

            template<class T>
            std::shared_ptr<T> GetComponentOfType() const
            {
                StaticAssertComponentIsBase<T>();
                return std::static_pointer_cast<T>(GetComponentOfTypeInner(typeid(T).hash_code()));
            }

            template<class T>
            std::vector<std::shared_ptr<T>> AddComponentsOfType()
            {
                StaticAssertComponentIsBase<T>();

                std::vector<Component> components = AddComponentsOfTypeInner(typeid(T).hash_code());
                std::vector<T> componentsAsT(components.size());
                std::transform(std::begin(components), std::end(components), std::begin(componentsAsT), [](const Component& component)
                {
                    return std::static_pointer_cast<T>(component);
                });
                return componentsAsT;
            }

            template<class T>
            std::vector<std::shared_ptr<T>> GetComponentsOfType() const
            {
                StaticAssertComponentIsBase<T>();

                std::vector<Component> components = GetComponentsOfTypeInner(typeid(T).hash_code());
                std::vector<T> componentsAsT(components.size());
                std::transform(std::begin(components), std::end(components), std::begin(componentsAsT), [](const Component& component)
                {
                    return std::static_pointer_cast<T>(component);
                });
                return componentsAsT;
            }

            template<class T>
            void RemoveComponentOfType()
            {
                StaticAssertComponentIsBase<T>();
                RemoveComponentOfTypeInner(typeid(T).hash_code());
            }

            template<class T>
            void RemoveComponentsOfType()
            {
                StaticAssertComponentIsBase<T>();
                RemoveComponentsOfTypeInner(typeid(T).hash_code());
            }

        private:

            std::shared_ptr<Component> AddComponentOfTypeInner(size_t typeToAdd);
            std::vector<std::shared_ptr<Component>> AddComponentsOfTypeInner(size_t typeToAdd);

            std::shared_ptr<Component> GetComponentOfTypeInner(size_t typeToFind) const;
            std::vector<std::shared_ptr<Component>> GetComponentsOfTypeInner(size_t typeToFind) const;

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
