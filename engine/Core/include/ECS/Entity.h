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
            size_t ComponentHash = 0;
            Component* Component = nullptr;
            std::function<void(Entity&)> RemoveComponent = {};
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

            void AddComponent(const ComponentData& componentData);

            template<class T>
#ifdef THREAD_SAFE
            const T* GetComponentOfType(size_t typeToFind = 0) const
#else
            T* GetComponentOfType(size_t typeToFind = 0) const
#endif
            {
                if (typeToFind == 0)
                {
                    typeToFind = typeid(T).hash_code();
                }

                T* foundType = nullptr;
                auto it = std::begin(m_components);
                while (!foundType && it != std::end(m_components))
                {
                    if (typeToFind == it->ComponentHash)
                    {
                        foundType = static_cast<T*>(it->Component);
                    }
                    it++;
                }
                return foundType;
            }

            template<class T>
#ifdef THREAD_SAFE
            std::vector<const T*> GetComponentsOfType(size_t typeToFind = 0) const
            {
                std::vector<const T*> foundTypes;
#else
                std::vector<T*> GetComponentsOfType(size_t typeToFind = 0) const
                {
                    std::vector<T*> foundTypes;
#endif
                if (typeToFind == 0)
                {
                    typeToFind = typeid(T).hash_code();
                }

                foundTypes.push_back(GetComponentOfType<T>(typeToFind));
                for (Entity* child : m_children)
                {
                    auto childFoundTypes = child->GetComponentsOfType<T>(typeToFind);
                    foundTypes.insert(std::end(foundTypes), std::begin(childFoundTypes), std::end(childFoundTypes));
                }
                return foundTypes;
            }

            template<class T>
            void RemoveComponentOfType(size_t typeToFind = 0)
            {
                if (typeToFind == 0)
                {
                    typeToFind = typeid(T).hash_code();
                }

                bool foundComponent = false;
                auto it = std::begin(m_components);
                while (!foundComponent && it != std::end(m_components))
                {
                    if (typeToFind == it->ComponentHash)
                    {
                        foundComponent = true;
                    }
                    else
                    {
                        it++;
                    }
                }
                if (foundComponent)
                {
                    it->RemoveComponent(*this);
                    m_components.erase(it);
                }
            }

            template<class T>
            void RemoveComponentsOfType(size_t typeToFind = 0)
            {
                if (typeToFind == 0)
                {
                    typeToFind = typeid(T).hash_code();
                }

                RemoveComponentOfType<T>(typeToFind);
                for (Entity* child : m_children)
                {
                    child->RemoveComponentsOfType<T>(typeToFind);
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
