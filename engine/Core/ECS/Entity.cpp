#include "stdafx.h"

#include "ECS/Entity.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        unsigned int Entity::s_numEntities = 0;
        unsigned int Entity::s_maxDiscardedEntityIDs = 128;
        std::list<unsigned int> Entity::s_reusableEntityIDs;
        std::vector<ComponentManagerData> Entity::s_componentManagers;

        Entity::Entity(Entity* parent) :
            m_parent(parent)
        {
            if (m_parent)
            {
                m_parent->m_children.push_back(this);
                m_enabled = m_parent->IsEnabled();
            }
            else
            {
                m_enabled = true;
            }

            if (s_maxDiscardedEntityIDs > s_reusableEntityIDs.size())
            {
                m_id = s_numEntities++;
            }
            else
            {
                m_id = s_reusableEntityIDs.front();
                s_reusableEntityIDs.erase(std::begin(s_reusableEntityIDs));
            }
        }

        Entity::~Entity()
        {
            s_numEntities--;
            s_reusableEntityIDs.push_back(m_id);

            for (auto it = std::begin(s_componentManagers); it != std::end(s_componentManagers); it++)
            {
                it->RemoveComponent(*this);
            }

            for (Entity* child : m_children)
            {
                delete child;
                child = nullptr;
            }
        }

        unsigned int Entity::GetID() const
        {
            return m_id;
        }

        unsigned int Entity::GetEntityCount()
        {
            return s_numEntities;
        }

        Entity* Entity::GetParent() const
        {
            return m_parent;
        }

        void Entity::SetParent(Entity* parent)
        {
            m_parent = parent;
            SetEnabled(m_parent->IsEnabled());
        }


        const std::vector<Entity*>& Entity::GetChildren() const
        {
            return m_children;
        }

        const Math::Transform& Entity::GetLocalTransform() const
        {
            return m_localTransform;
        }

        void Entity::SetLocalTransform(const Math::Transform& transform)
        {
            m_localTransform = transform;
        }

        Math::Transform Entity::GetWorldTransform() const
        {
            return m_parent ? m_parent->GetWorldTransform() * m_localTransform : m_localTransform;
        }

        void Entity::SetWorldTransform(const Math::Transform& transform)
        {
            m_localTransform = GetWorldTransform() * transform;
        }

        void Entity::RegisterComponentManager(const ComponentManagerData& componentManagerData)
        {
            bool componentManagerFound = false;

            auto it = std::begin(s_componentManagers);
            while (!componentManagerFound && it != std::end(s_componentManagers))
            {
                componentManagerFound = it->ManagedTypeHash == componentManagerData.ManagedTypeHash;
                it++;
            }

            if (!componentManagerFound)
            {
                s_componentManagers.push_back(componentManagerData);
            }
        }

        Component* Entity::AddComponentOfTypeInner(size_t typeToAdd)
        {
            Component* component = GetComponentOfTypeInner(typeToAdd);
            if (!component)
            {
                bool addedComponent = false;
                auto componentManagerIt = std::begin(s_componentManagers);
                while (!addedComponent && componentManagerIt != std::end(s_componentManagers))
                {
                    if (componentManagerIt->ManagedTypeHash == typeToAdd)
                    {
                        ComponentData data;
                        data.Component = componentManagerIt->AddComponent(*this);
                        data.ComponentTypeHash = typeToAdd;
                        m_components.push_back(data);
                        addedComponent = true;
                        component = data.Component;
                    }
                    componentManagerIt++;
                }
            }
            return component;
        }

        std::vector<Component*> Entity::AddComponentsOfTypeInner(size_t typeToAdd)
        {
            AddComponentOfTypeInner(typeToAdd);
            for (Entity* child : m_children)
            {
                child->AddComponentsOfTypeInner(typeToAdd);
            }
        }

        void Entity::SetEnabled(bool enabled)
        {
            if (m_enabled != enabled)
            {
                m_enabled = enabled;
                for (auto it = std::begin(m_components); it != std::end(m_components); it++)
                {
                    it->Component->SetEnabled(enabled);
                }
            }

            for (Entity* child : m_children)
            {
                child->SetEnabled(enabled);
            }
        }

        bool Entity::IsEnabled() const
        {
            return m_enabled;
        }

        void Entity::SwapID(Entity& e)
        {
            unsigned int temp = m_id;
            m_id = e.GetID();
            e.m_id = temp;
        }

        const EntityBitset& Entity::GetLayers() const
        {
            return m_layers;
        }

        void Entity::SetLayers(const EntityBitset& layer)
        {
            m_layers = layer;
        }

        const std::string& Entity::GetName() const
        {
            return m_name;
        }

        void Entity::SetName(const std::string& name)
        {
            m_name = name;
        }

        const std::vector<std::string>& Entity::GetTags() const
        {
            return m_tags;
        }

        void Entity::SetTags(const std::vector<std::string>& tags)
        {
            m_tags = tags;
        }

        Component* Entity::GetComponentOfTypeInner(size_t typeToFind) const
        {
            Component* foundType = nullptr;
            auto it = std::begin(m_components);
            while (!foundType && it != std::end(m_components))
            {
                if (typeToFind == it->ComponentTypeHash)
                {
                    foundType = it->Component;
                }
                it++;
            }
            return foundType;
        }

        std::vector<Component*> Entity::GetComponentsOfTypeInner(size_t typeToFind = 0) const
        {
            std::vector<Component*> foundTypes;
            foundTypes.push_back(GetComponentOfTypeInner(typeToFind));
            for (Entity* child : m_children)
            {
                auto childFoundTypes = child->GetComponentsOfTypeInner(typeToFind);
                foundTypes.insert(std::end(foundTypes), std::begin(childFoundTypes), std::end(childFoundTypes));
            }
            return foundTypes;
        }

        void Entity::RemoveComponentOfTypeInner(size_t typeToFind)
        {
            bool componentManagerFound = false;
            auto managerIt = std::begin(s_componentManagers);
            while (!componentManagerFound && managerIt != std::end(s_componentManagers))
            {
                if (managerIt->ManagedTypeHash == typeToFind)
                {
                    componentManagerFound = true;
                    bool foundComponent = false;
                    auto componentIt = std::begin(m_components);
                    while (!foundComponent && componentIt != std::end(m_components))
                    {
                        if (componentIt->ComponentTypeHash == typeToFind)
                        {
                            foundComponent = true;
                        }
                        else
                        {
                            componentIt++;
                        }
                    }

                    if (componentIt != std::end(m_components))
                    {
                        m_components.erase(componentIt);
                        managerIt->RemoveComponent(*this);
                    }
                }
                managerIt++;
            }
        }

        void Entity::RemoveComponentsOfTypeInner(size_t typeToFind)
        {
            RemoveComponentOfTypeInner(typeToFind);
            for (Entity* child : m_children)
            {
                child->RemoveComponentsOfTypeInner(typeToFind);
            }
        }
    }
}
