#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>
#include <algorithm>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class Entity;
        class Component;
        typedef std::bitset<32> EntityBitset;

        struct ComponentManagerData
        {
            size_t ManagerTypeHash = 0;
            std::function<std::shared_ptr<Component>(Entity&)> AddComponent = {};
            std::function<void(Entity&)> RemoveComponent = {};
        };

        template<class T>
        inline void StaticAssertComponentIsBase()
        {
            static_assert(std::is_base_of<Component, T>::value, "T must be a child of Component");
        }

        class CAERULUS_CORE Entity final : Interface::NonCopyable
        {
        public:
            Entity(std::shared_ptr<Entity> parent = nullptr);
            ~Entity();

            uint32_t GetID() const;

            std::shared_ptr<Entity> GetParent() const;
            void SetParent(std::shared_ptr<Entity> parent);

            const std::vector<std::shared_ptr<Entity>>& GetChildren() const;

            std::string_view GetName() const;
            void SetName(std::string_view layer);

            bool IsEnabled() const;
            void SetEnabled(bool enabled);

            EntityBitset& GetLayers();
            const EntityBitset& GetLayers() const;

            std::vector<std::string>& GetTags();
            const std::vector<std::string>& GetTags() const;

            Math::Transform& GetLocalTransform();
            const Math::Transform& GetLocalTransform() const;

            Math::Transform GetWorldTransform() const;
            void SetWorldTransform(const Math::Transform& transform);

            void SwapID(Entity& e);
            std::vector<std::shared_ptr<Entity>> ToVector() const;

            static uint32_t GetEntityCount();
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

            bool m_isDeleted = false;
            bool m_isEnabled = false;
            uint32_t m_id  = 0;
            std::string m_name = "";
            EntityBitset m_layers;
            Math::Transform m_localTransform;
            std::vector<std::string> m_tags;

            std::weak_ptr<Entity> m_parent;
            std::vector<std::shared_ptr<Entity>> m_children;

            struct ComponentData
            {
                size_t ComponentTypeHash = 0;
                std::shared_ptr<Component> Component;
            };
            std::vector<ComponentData> m_components;

            // This approach needs to be re-thought
            static std::vector<ComponentManagerData> s_componentManagers;

            // TODO - move this to a scene class
            static uint32_t s_numEntities;
            static uint32_t s_maxDiscardedEntityIDs;
            static std::list<uint32_t> s_reusableEntityIDs;
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
