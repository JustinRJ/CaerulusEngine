#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <set>
#include <list>
#include <memory>
#include <string>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class CAERULUS_CORE Node : Interface::NonCopyable
        {
        public:
            Node(Node* parent);
            virtual ~Node();

            bool IsRoot() const;

            unsigned int GetID() const;
            unsigned int GetNodeCount() const;

            const std::string& GetLayer() const;
            void SetLayer(const std::string& layer);

            const std::string& GetName() const;
            void SetName(const std::string& layer);

            Math::Transform GetWorldTransform() const;
            void SetWorldTransform(const Math::Transform& transform);

            const Math::Transform& GetLocalTransform() const;
            void SetLocalTransform(const Math::Transform& transform);

            void SwapID(Node& e);

        private:
            void SetID(unsigned int id);
            void CalculateWorldSpace(Math::Transform& ctm) const;

            unsigned int m_ID;
            static unsigned int s_numEntities;
            static unsigned int s_maxDiscardedNodeIDs;
            static std::list<unsigned int> s_reusableNodeIDs;

            Node* m_parent;
            std::set<Node*> m_children;

            std::string m_name;
            std::string m_layer;
            Math::Transform m_localTransform;
        };

        inline bool operator== (const Node& left, const Node& right)
        {
            return left.GetID() == right.GetID();
        }

        inline bool operator< (const Node& left, const Node& right)
        {
            return left.GetID() < right.GetID();
        }

        inline bool operator> (const Node& left, const Node& right)
        {
            return right < left;
        }

        inline bool operator<= (const Node& left, const Node& right)
        {
            return !(left > right);
        }

        inline bool operator>= (const Node& left, const Node& right)
        {
            return !(left < right);
        }
    }
}
