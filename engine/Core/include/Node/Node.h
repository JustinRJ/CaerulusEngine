#pragma once

#define CAERULUS_CORE __declspec(dllexport)

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class CAERULUS_CORE Node : private Interface::NonCopyable
        {
        public:
            Node(std::shared_ptr<Node> parent = nullptr /* Todo */);
            virtual ~Node();

            bool IsRoot() const;

            unsigned int GetID() const;
            unsigned int GetNodeCount() const;

            const std::string& GetLayer() const;
            void SetLayer(const std::string& layer);

            const std::string& GetName() const;
            void SetName(const std::string& layer);

            Math::Transform& GetTransform();
            const Math::Transform& GetTransform() const;

            void SwapID(Node& e);

        private:
            void SetID(unsigned int id);

            static unsigned int s_numEntities;
            static unsigned int s_maxDiscardedNodeIDs;
            static std::list<unsigned int> s_reusableNodeIDs;

            unsigned int m_ID;
            std::string m_name;
            std::string m_layer;
            Math::Transform m_transform;

            Node* m_parent;
            std::vector<Node*> m_children;
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