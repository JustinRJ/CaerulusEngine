#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Node
    {
        class Node;

        class CAERULUS_CORE Component : Interface::NonCopyable
        {
        public:
            Component(Node& node) :
                m_node(node)
            {}

            virtual ~Component() = default;

            virtual void OnEnable() {};
            virtual void OnDisable() {};

            virtual void EarlyUpdate() {};
            virtual void Update(float deltaTime) {};
            virtual void FixedUpdate(float fixedTime) {};
            virtual void LateUpdate() {};

            virtual void Reset() {};

            Node& GetNode()
            {
                return m_node;
            }

            const Node& GetNode() const
            {
                return m_node;
            }

        protected:
            Node& m_node;
        };
    }
}
