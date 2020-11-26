#pragma once

#include "Core/Math/Transform.h"
#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class Node : public Interface::NonCopyable
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

            ~Node()
            {
                s_numEntities--;
                s_reusableNodeIDs.push_back(m_ID);
            }

            unsigned int GetID() const
            {
                return m_ID;
            }

            unsigned int GetNodeCount() const
            {
                return s_numEntities;
            }

            Math::Transform& GetTransform()
            {
                return m_transform;
            }

            const Math::Transform& GetTransform() const
            {
                return m_transform;
            }

            void SwapID(Node& e)
            {
                unsigned int temp = m_ID;
                m_ID = e.GetID();
                e.SetID(temp);
            }

        private:
            void SetID(unsigned int id)
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

        bool operator== (const Node &left, const Node &right)
        {
            return left.GetID() == right.GetID();
        }

        bool operator< (const Node &left, const Node &right)
        {
            return left.GetID() < right.GetID();
        }

        bool operator> (const Node &left, const Node &right)
        {
            return right < left;
        }

        bool operator<= (const Node &left, const Node &right)
        {
            return !(left > right);
        }

        bool operator>= (const Node &left, const Node &right)
        {
            return !(left < right);
        }
    }
}
