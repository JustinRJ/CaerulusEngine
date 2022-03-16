#pragma once

#include <list>
#include <vector>
#include <bitset>
#include <string>

#include "Math/Transform.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class Node;
        class Component;

        struct ComponentData
        {
            Component* Component;
            std::function<void(const Node&)> OnDestroy;
        };

        typedef std::bitset<32> NodeBitset;

        class CAERULUS_CORE Node : Interface::NonCopyable
        {
        public:
            Node(Node* parent);
            ~Node();

            unsigned int GetID() const;

            unsigned int GetNodeCount() const;

            Node* GetParent() const;
            const std::vector<Node*>& GetChildren() const;

            const std::string& GetName() const;
            void SetName(const std::string& layer);

            bool GetEnabled() const;
            void SetEnabled(bool enabled);

            const NodeBitset& GetLayers() const;
            void SetLayers(const NodeBitset& layer);

            const std::vector<std::string>& GetTags() const;
            void SetTags(const std::vector<std::string>& tags);

            Math::Transform GetWorldTransform() const;
            void SetWorldTransform(const Math::Transform& transform);

            const Math::Transform& GetLocalTransform() const;
            void SetLocalTransform(const Math::Transform& transform);

            void SwapID(Node& e);

            void AddComponent(ComponentData componentData);
            void RemoveComponent(const Component& component);

            template<class T>
            T* GetComponentOfType()
            {
                T* foundType = nullptr;
                auto it = std::begin(m_components);
                while (!foundType && it != std::end(m_components))
                {
                    if (auto type = dynamic_cast<T*>(it->Component))
                    {
                        foundType = type;
                    }
                    it++;
                }
                return foundType;
            }

            template<class T>
            std::vector<T*> GetComponentsOfType()
            {
                std::vector<T*> foundTypes;
                foundTypes.push_back(GetComponentOfType<T>());
                for (Node* child : m_children)
                {
                    auto childFoundTypes = child->GetComponentsOfType<T>();
                    foundTypes.insert(std::end(foundTypes), std::begin(childFoundTypes), std::end(childFoundTypes));
                }
                return foundTypes;
            }

            template<class T>
            void RemoveComponentOfType()
            {
                if (auto component = GetComponentOfType<T>())
                {
                    RemoveComponent(*component);
                }
            }

            template<class T>
            void RemoveComponentsOfType()
            {
                RemoveComponentOfType<T>();
                for (Node* child : m_children)
                {
                    child->RemoveComponentsOfType<T>();
                }
            }

        private:
            bool m_enabled;
            unsigned int m_ID;
            std::string m_name;
            NodeBitset m_layers;
            std::vector<std::string> m_tags;

            Node* m_parent;
            std::vector<Node*> m_children;

            Math::Transform m_localTransform;
            std::vector<ComponentData> m_components;

            static unsigned int s_numEntities;
            static unsigned int s_maxDiscardedNodeIDs;
            static std::list<unsigned int> s_reusableNodeIDs;
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
