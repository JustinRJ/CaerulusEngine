#include "stdafx.h"

#include "Node.h"
#include <list>

namespace
{
    unsigned int NumEntities = 0;
    unsigned int MaxDiscardedNodeIDs = 64;
    std::list<unsigned int> ReusableNodeIDs;
}

namespace Core
{
    namespace Node
    {
        Node::Node()
        {
            if (ReusableNodeIDs.size() < MaxDiscardedNodeIDs)
            {
                m_ID = NumEntities++;
            }
            else
            {
                m_ID = ReusableNodeIDs.front();
                ReusableNodeIDs.erase(ReusableNodeIDs.begin());
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
            NumEntities--;
            ReusableNodeIDs.push_back(m_ID);
        }

        unsigned int Node::GetID() const
        {
            return m_ID;
        }

        unsigned int Node::GetNodeCount() const
        {
            return NumEntities;
        }

        void Node::SetID(unsigned int id)
        {
            m_ID = id;
        }
    }
}