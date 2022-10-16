#include "stdafx.h"

#include "ECS/Entity.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        uint32_t Entity::s_numEntities = 0;
        uint32_t Entity::s_maxDiscardedEntityIDs = 1000;
        std::list<uint32_t> Entity::s_reusableEntityIDs;
        std::vector<ComponentManagerData> Entity::s_componentManagers;

        Entity::Entity(std::shared_ptr<Entity> parent) :
            m_parent(parent)
        {
            if (auto p = m_parent.lock())
            {
                p->m_children.push_back(std::shared_ptr<Entity>(this));
                m_isEnabled = p->IsEnabled();
            }
            else
            {
                m_isEnabled = true;
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

            m_isDeleted = true;
            m_children.clear();

            for (auto& componentData : s_componentManagers)
            {
                componentData.RemoveComponent(*this);
            }

            auto p = m_parent.lock();
            if (p && !p->m_isDeleted)
            {
                auto& pChildren = p->m_children;
                pChildren.erase(std::find_if(pChildren.begin(), pChildren.end(), [&](const std::shared_ptr<Entity>& e)
                {
                    return e->GetID() == GetID();
                }));
            }
        }

        uint32_t Entity::GetID() const
        {
            return m_id;
        }

        uint32_t Entity::GetEntityCount()
        {
            return s_numEntities;
        }

        std::shared_ptr<Entity> Entity::GetParent() const
        {
            return m_parent.lock();
        }

        void Entity::SetParent(std::shared_ptr<Entity> parent)
        {
            if (auto p = m_parent.lock())
            {
                auto& pChildren = p->m_children;
                pChildren.erase(std::find_if(pChildren.begin(), pChildren.end(), [&](const std::shared_ptr<Entity>& e)
                {
                    return e->GetID() == GetID();
                }));
            }
            m_parent = parent;

            if (auto p = m_parent.lock())
            {
                p->m_children.push_back(std::shared_ptr<Entity>(this));
                SetEnabled(p->IsEnabled());
            }
        }

        const std::vector<std::shared_ptr<Entity>>& Entity::GetChildren() const
        {
            return m_children;
        }

        Math::Transform& Entity::GetLocalTransform()
        {
            return m_localTransform;
        }

        const Math::Transform& Entity::GetLocalTransform() const
        {
            return m_localTransform;
        }

        Math::Transform Entity::GetWorldTransform() const
        {
            return m_parent.lock() ? m_parent.lock()->GetWorldTransform() * m_localTransform : m_localTransform;
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
                componentManagerFound = it->ManagerTypeHash == componentManagerData.ManagerTypeHash;
                it++;
            }

            if (!componentManagerFound)
            {
                s_componentManagers.push_back(componentManagerData);
            }
        }

        std::shared_ptr<Component> Entity::AddComponentOfTypeInner(size_t typeToAdd)
        {
            std::shared_ptr<Component> component = GetComponentOfTypeInner(typeToAdd);
            if (!component)
            {
                bool addedComponent = false;
                auto componentManagerIt = std::begin(s_componentManagers);
                while (!addedComponent && componentManagerIt != std::end(s_componentManagers))
                {
                    if (componentManagerIt->ManagerTypeHash == typeToAdd)
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

        std::vector<std::shared_ptr<Component>> Entity::AddComponentsOfTypeInner(size_t typeToAdd)
        {
            std::vector<std::shared_ptr<Component>> addedComponents;
            addedComponents.push_back(AddComponentOfTypeInner(typeToAdd));
            for (std::shared_ptr<Entity>& child : m_children)
            {
                auto childAddedComponents = child->AddComponentsOfTypeInner(typeToAdd);
                addedComponents.insert(std::end(addedComponents), std::begin(childAddedComponents), std::end(childAddedComponents));
            }
            return addedComponents;
        }

        void Entity::SetEnabled(bool enabled)
        {
            if (m_isEnabled != enabled)
            {
                m_isEnabled = enabled;

                for (ComponentData& componentData : m_components)
                {
                    componentData.Component->SetEnabled(enabled);
                }
            }

            for (std::shared_ptr<Entity>& child : m_children)
            {
                child->SetEnabled(enabled);
            }
        }

        bool Entity::IsEnabled() const
        {
            return m_isEnabled;
        }

        void Entity::SwapID(Entity& e)
        {
            uint32_t temp = m_id;
            m_id = e.m_id;
            e.m_id = temp;
        }

        std::vector<std::shared_ptr<Entity>> Entity::ToVector() const
        {
            std::vector<std::shared_ptr<Entity>> vec;
            vec.insert(vec.begin(), m_children.begin(), m_children.end());
            for (auto child : m_children)
            {
                vec.insert(vec.begin(), child->m_children.begin(), child->m_children.end());
            }
            return vec;
        }

        EntityBitset& Entity::GetLayers()
        {
            return m_layers;
        }

        const EntityBitset& Entity::GetLayers() const
        {
            return m_layers;
        }

        std::string_view Entity::GetName() const
        {
            return m_name;
        }

        void Entity::SetName(std::string_view name)
        {
            m_name = name;
        }

        std::vector<std::string>& Entity::GetTags()
        {
            return m_tags;
        }

        const std::vector<std::string>& Entity::GetTags() const
        {
            return m_tags;
        }

        std::shared_ptr<Component> Entity::GetComponentOfTypeInner(size_t typeToFind) const
        {
            std::shared_ptr<Component> foundType = nullptr;
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

        std::vector<std::shared_ptr<Component>> Entity::GetComponentsOfTypeInner(size_t typeToFind = 0) const
        {
            std::vector<std::shared_ptr<Component>> foundTypes;
            foundTypes.push_back(GetComponentOfTypeInner(typeToFind));
            for (const std::shared_ptr<Entity>& child : m_children)
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
                if (managerIt->ManagerTypeHash == typeToFind)
                {
                    componentManagerFound = true;

                    auto it = std::find_if(std::begin(m_components), std::end(m_components),
                        [typeToFind](const ComponentData& componentData)
                    {
                        return componentData.ComponentTypeHash == typeToFind;
                    });

                    if (it != std::end(m_components))
                    {
                        m_components.erase(it);
                        managerIt->RemoveComponent(*this);
                    }
                }
                managerIt++;
            }
        }

        void Entity::RemoveComponentsOfTypeInner(size_t typeToFind)
        {
            RemoveComponentOfTypeInner(typeToFind);
            for (std::shared_ptr<Entity>& child : m_children)
            {
                child->RemoveComponentsOfTypeInner(typeToFind);
            }
        }
    }
}
