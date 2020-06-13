#pragma once

#include "INode.h"
#include <list>
#include "Core/Math/Transform.h"

namespace Core
{
    namespace Node
    {
        class Node : public INode
        {
        public:

            Node()
            {
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

            virtual ~Node()
            {
                s_numEntities--;
                s_reusableNodeIDs.push_back(m_ID);
            }

            unsigned int GetID() const final
            {
                return m_ID;
            }

            unsigned int GetNodeCount() const final
            {
                return s_numEntities;
            }

            const Math::Transform& GetTransform() const final
            {
                return m_transform;
            }

            void SwapID(INode& e) final
            {
                unsigned int temp = m_ID;
                m_ID = e.GetID();
                dynamic_cast<Node&>(e).SetID(temp);
            }

        private:
            void SetID(unsigned int id) final
            {
                m_ID = id;
            }

            static unsigned int s_numEntities;
            static unsigned int s_maxDiscardedNodeIDs;
            static std::list<unsigned int> s_reusableNodeIDs;

            unsigned int m_ID;
            Core::Math::Transform m_transform;
        };

        unsigned int Node::s_numEntities = 0;
        unsigned int Node::s_maxDiscardedNodeIDs = 128;
        std::list<unsigned int> Node::s_reusableNodeIDs;

        bool operator == (const INode &left, const INode &right)
        {
            return left.GetID() == right.GetID();
        }

        bool operator < (const INode &left, const INode &right)
        {
            return left.GetID() < right.GetID();
        }

        bool operator > (const INode &left, const INode &right)
        {
            return right < left;
        }

        bool operator <= (const INode &left, const INode &right)
        {
            return !(left > right);
        }

        bool operator >= (const INode &left, const INode &right)
        {
            return !(left < right);
        }
    }
}
