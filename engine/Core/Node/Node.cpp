#include "stdafx.h"

#include "Node.h"

namespace Core
{
    namespace Node
    {
        unsigned int Node::s_numEntities = 0;
        unsigned int Node::s_maxDiscardedNodeIDs = 128;
        std::list<unsigned int> Node::s_reusableNodeIDs;

        Node::Node(std::shared_ptr<Node> parent) :
            m_parent(parent.get())
        {
            if (m_parent)
            {
                m_parent->m_children.push_back(this);
            }

            if (s_reusableNodeIDs.size() < s_maxDiscardedNodeIDs)
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

        unsigned int Node::GetID() const
        {
            return m_ID;
        }

        unsigned int Node::GetNodeCount() const
        {
            return s_numEntities;
        }

        Math::Transform& Node::GetTransform()
        {
            return m_transform;
        }

        const Math::Transform& Node::GetTransform() const
        {
            return m_transform;
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
