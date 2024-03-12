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
        class IComponentManager;
        typedef std::bitset<32> EntityBitset;

        class CAERULUS_CORE Entity final : Interface::NonCopyable
        {
        public:
            Entity(std::shared_ptr<Entity> parent = nullptr);
            ~Entity();

            uint32_t GetID() const;

            std::shared_ptr<Entity> GetParent() const;
            void SetParent(std::shared_ptr<Entity> parent);

            const std::vector<std::shared_ptr<Entity>>& GetChildren() const;

            void RemoveChild(const Entity& e);

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
            static void RegisterComponentManager(IComponentManager* componentManagerData);
            static void DeregisterComponentManager(IComponentManager* componentManagerData);

            template<class T>
            T* AddComponentOfType()
            {
                return static_cast<T*>(AddComponentOfTypeInner(typeid(T).hash_code()));
            }

            template<class T>
            T* GetComponentOfType() const
            {
                return static_cast<T*>(GetComponentOfTypeInner(typeid(T).hash_code()));
            }

            template<class T>
            std::vector<T*> AddComponentsOfType()
            {
                std::vector<void*> components = AddComponentsOfTypeInner(typeid(T).hash_code());
                std::vector<T*> componentsAsT(components.size());
                std::transform(std::begin(components), std::end(components), std::begin(componentsAsT), [](void* component)
                {
                    return static_cast<T*>(component);
                });
                return componentsAsT;
            }

            template<class T>
            std::vector<T*> GetComponentsOfType() const
            {
                std::vector<void*> components = GetComponentsOfTypeInner(typeid(T).hash_code());
                std::vector<T*> componentsAsT(components.size());
                std::transform(std::begin(components), std::end(components), std::begin(componentsAsT), [](void* component)
                {
                    return static_cast<T*>(component);
                });
                return componentsAsT;
            }

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

            void* AddComponentOfTypeInner(size_t typeToAdd);
            std::vector<void*> AddComponentsOfTypeInner(size_t typeToAdd);

            void* GetComponentOfTypeInner(size_t typeToFind);
            std::vector<void*> GetComponentsOfTypeInner(size_t typeToFind);

            void RemoveComponentOfTypeInner(size_t typeToRemove);
            void RemoveComponentsOfTypeInner(size_t typeToRemove);

            bool m_isDeleted = false;
            bool m_isEnabled = false;
            uint32_t m_id;
            std::string m_name;
            EntityBitset m_layers;
            Math::Transform m_localTransform;
            std::vector<std::string> m_tags;

            std::weak_ptr<Entity> m_parent;
            std::vector<std::shared_ptr<Entity>> m_children;

            // TODO - move this to a EntityManager class
            static std::vector<IComponentManager*> s_componentManagers;
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
