#include "stdafx.h"

#include "Node/Node.h"
#include "Node/Component.h"

#include <vector>

namespace Core
{
    namespace Node
    {
        unsigned int Node::s_numEntities = 0;
        unsigned int Node::s_maxDiscardedNodeIDs = 128;
        std::list<unsigned int> Node::s_reusableNodeIDs;

        Node::Node(Node* parent) :
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

            if (s_maxDiscardedNodeIDs > s_reusableNodeIDs.size())
            {
                m_ID = s_numEntities++;
            }
            else
            {
                m_ID = s_reusableNodeIDs.front();
                s_reusableNodeIDs.erase(std::begin(s_reusableNodeIDs));
            }
        }

        Node::~Node()
        {
            s_numEntities--;
            s_reusableNodeIDs.push_back(m_ID);

            for (auto it = std::begin(m_components); it != std::end(m_components); it++)
            {
                it->Component->OnDisable();
                it->OnDestroy(*this);
            }

            for (Node* child : m_children)
            {
                delete child;
            }
        }

        unsigned int Node::GetID() const
        {
            return m_ID;
        }

        unsigned int Node::GetNodeCount() const
        {
            return s_numEntities;
        }

        Node* Node::GetParent() const
        {
            return m_parent;
        }

        const std::vector<Node*>& Node::GetChildren() const
        {
            return m_children;
        }

        const Math::Transform& Node::GetLocalTransform() const
        {
            return m_localTransform;
        }

        void Node::SetLocalTransform(const Math::Transform& transform)
        {
            m_localTransform = transform;
        }

        Math::Transform Node::GetWorldTransform() const
        {
            return m_parent ? m_parent->GetWorldTransform() * m_localTransform : m_localTransform;
        }

        void Node::SetWorldTransform(const Math::Transform& transform)
        {
            m_localTransform = GetWorldTransform() * transform;
        }

        void Node::AddComponent(const ComponentData componentData)
        {
            m_components.push_back(componentData);
        }

        void Node::RemoveComponent(const Component& component)
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

        void Node::SetEnabled(bool enabled)
        {
            if (m_enabled != enabled)
            {
                m_enabled = enabled;
                for (auto it = std::begin(m_components); it != std::end(m_components); it++)
                {
                    enabled ? it->Component->OnEnable() : it->Component->OnDisable();
                }
            }

            for (Node* child : m_children)
            {
                child->SetEnabled(enabled);
            }
        }

        bool Node::GetEnabled() const
        {
            return m_enabled;
        }

        void Node::SwapID(Node& e)
        {
            unsigned int temp = m_ID;
            m_ID = e.GetID();
            e.m_ID = temp;
        }

        const NodeBitset& Node::GetLayers() const
        {
            return m_layers;
        }

        void Node::SetLayers(const NodeBitset& layer)
        {
            m_layers = layer;
        }

        const std::string& Node::GetName() const
        {
            return m_name;
        }

        void Node::SetName(const std::string& name)
        {
            m_name = name;
        }

        const std::vector<std::string>& Node::GetTags() const
        {
            return m_tags;
        }

        void Node::SetTags(const std::vector<std::string>& tags)
        {
            m_tags = tags;
        }
    }
}
