#include "stdafx.h"

#include "Node/Node.h"

#include <vector>

namespace Core
{
    namespace Node
    {
        unsigned int Node::s_numEntities = 0;
        unsigned int Node::s_maxDiscardedNodeIDs = 128;
        std::list<unsigned int> Node::s_reusableNodeIDs;

        Node::Node(Node* parent) :
            m_parent(parent),
            m_localTransform(Math::Transform())
        {
            if (m_parent)
            {
                m_parent->m_children.insert(this);
            }

            if (s_maxDiscardedNodeIDs > s_reusableNodeIDs.size())
            {
                m_ID = s_numEntities++;
            }
            else
            {
                m_ID = s_reusableNodeIDs.front();
                s_reusableNodeIDs.erase(s_reusableNodeIDs.begin());
            }
        }

        Node::~Node()
        {
            s_numEntities--;
            s_reusableNodeIDs.push_back(m_ID);
        }

        bool Node::IsRoot() const
        {
            return !m_parent;
        }

        unsigned int Node::GetID() const
        {
            return m_ID;
        }

        unsigned int Node::GetNodeCount() const
        {
            return s_numEntities;
        }

        const Math::Transform& Node::GetLocalTransform() const
        {
            return m_localTransform;
        }

        void Node::SetLocalTransform(const Math::Transform& transform)
        {
            m_localTransform = transform;
        }

        void Node::CalculateWorldSpace(Math::Transform& ctm) const
        {
            if (m_parent)
            {
                m_parent->CalculateWorldSpace(ctm);
            }
            ctm = ctm * m_localTransform;
        }

        Math::Transform Node::GetWorldTransform() const
        {
            Math::Transform ctm;
            CalculateWorldSpace(ctm);
            return ctm;
        }

        void Node::SetWorldTransform(const Math::Transform& transform)
        {
            Math::Transform ctm;
            CalculateWorldSpace(ctm);
            m_localTransform = ctm * transform;
        }

        void Node::SwapID(Node& e)
        {
            unsigned int temp = m_ID;
            m_ID = e.GetID();
            e.SetID(temp);
        }

        void Node::SetID(unsigned int id)
        {
            m_ID = id;
        }

        const std::string& Node::GetLayer() const
        {
            return m_layer;
        }

        void Node::SetLayer(const std::string& layer)
        {
            m_layer = layer;
        }

        const std::string& Node::GetName() const
        {
            return m_name;
        }

        void Node::SetName(const std::string& name)
        {
            m_name = name;
        }
    }
}
