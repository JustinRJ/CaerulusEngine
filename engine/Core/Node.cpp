#include "stdafx.h"

#include "Node.h"
#include <list>

namespace
{
    unsigned int s_NumEntities = 0;
    unsigned int s_MaxDiscardedNodeIDs = 64;
    std::list<unsigned int> s_ReusableNodeIDs = std::list<unsigned int>(s_MaxDiscardedNodeIDs);
}

namespace Core
{
    Node::Node()
    {
        if (s_MaxDiscardedNodeIDs < s_ReusableNodeIDs.size())
        {
            m_ID = s_NumEntities++;
        }
        else
        {
            m_ID = s_ReusableNodeIDs.front();
            s_ReusableNodeIDs.erase(s_ReusableNodeIDs.begin());
        }
    }

    void Node::SwapID(INode& e)
    {
        unsigned int temp = m_ID;
        m_ID = e.GetID();
        dynamic_cast<Node&>(e).SetID(temp);
    }

    Node::~Node()
    {
        s_NumEntities--;
        s_ReusableNodeIDs.push_back(m_ID);
    }

    unsigned int Node::GetID() const
    {
        return m_ID;
    }

    unsigned int Node::GetNodeCount() const
    {
        return s_NumEntities;
    }

    void Node::SetID(unsigned int id)
    {
        m_ID = id;
    }
}