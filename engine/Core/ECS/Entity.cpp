#include "stdafx.h"

#include "ECS/Entity.h"
#include "ECS/Component.h"

#include <vector>

namespace Core
{
    namespace ECS
    {
        unsigned int Entity::s_numEntities = 0;
        unsigned int Entity::s_maxDiscardedEntityIDs = 128;
        std::list<unsigned int> Entity::s_reusableEntityIDs;

        Entity::Entity(Entity* parent) :
            m_parent(parent)
        {
            if (m_parent)
            {
                m_parent->m_children.push_back(this);
                m_enabled = parent->m_enabled;
            }
            else
            {
                m_enabled = true;
            }

            if (s_maxDiscardedEntityIDs > s_reusableEntityIDs.size())
            {
                m_ID = s_numEntities++;
            }
            else
            {
                m_ID = s_reusableEntityIDs.front();
                s_reusableEntityIDs.erase(std::begin(s_reusableEntityIDs));
            }
        }

        Entity::~Entity()
        {
            s_numEntities--;
            s_reusableEntityIDs.push_back(m_ID);

            for (auto it = std::begin(m_components); it != std::end(m_components); it++)
            {
                it->Component->OnDisable();
                it->OnDestroy(*this);
            }

            for (Entity* child : m_children)
            {
                delete child;
            }
        }

        unsigned int Entity::GetID() const
        {
            return m_ID;
        }

        unsigned int Entity::GetEntityCount() const
        {
            return s_numEntities;
        }

        Entity* Entity::GetParent() const
        {
            return m_parent;
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

        void Entity::AddComponent(const ComponentData& componentData)
        {
            m_components.push_back(componentData);
        }

        void Entity::RemoveComponent(const Component& component)
        {
            bool foundComponent = false;
            auto it = std::begin(m_components);
            while (!foundComponent && it != std::end(m_components))
            {
                if (it->Component == &component)
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
                it->Component->OnDisable();
                it->OnDestroy(*this);
                m_components.erase(it);
            }
        }

        void Entity::SetEnabled(bool enabled)
        {
            if (m_enabled != enabled)
            {
                m_enabled = enabled;
                for (auto it = std::begin(m_components); it != std::end(m_components); it++)
                {
                    enabled ? it->Component->OnEnable() : it->Component->OnDisable();
                }
            }

            for (Entity* child : m_children)
            {
                child->SetEnabled(enabled);
            }
        }

        bool Entity::GetEnabled() const
        {
            return m_enabled;
        }

        void Entity::SwapID(Entity& e)
        {
            unsigned int temp = m_ID;
            m_ID = e.GetID();
            e.m_ID = temp;
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
    }
}
