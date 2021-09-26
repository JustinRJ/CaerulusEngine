#pragma once

#include <map>
#include <memory>
#include <vector>

#include "Node/Node.h"
#include "Logging/Log.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Template
    {
        template <class T>
        class NodeManager : Interface::NonCopyable
        {
        public:
            NodeManager() = default;
            virtual ~NodeManager() = default;

            std::shared_ptr<T> Get(std::shared_ptr<Node::Node> key) const
            {
                auto it = m_managedMap.find(key);
                if (it != m_managedMap.end())
                {
                    return it->second;
                }
                Logging::LogInDebug("Managed node object with ID" + std::to_string(key->GetID()) + " not found!");
                return nullptr;
            }

            std::vector<std::shared_ptr<T>> GetAll() const
            {
                std::vector<std::shared_ptr<T>> elements;
                std::transform(m_managedMap.begin(), m_managedMap.end(),
                    std::back_inserter(elements), [](auto &kv) { return kv.second; });
                return elements;
            }

            bool Remove(std::shared_ptr<Node::Node> key)
            {
                if (IsLoaded(key))
                {
                    return m_managedMap.erase(key) > 0;
                }
                return false;
            }

            bool IsLoaded(std::shared_ptr<Node::Node> key) const
            {
                return m_managedMap.find(key) != m_managedMap.end();
            }

            void Clear()
            {
                m_managedMap.clear();
            }

            void Insert(std::shared_ptr<Node::Node> key, std::shared_ptr<T> value)
            {
                if (value)
                {
                    m_managedMap.insert(std::make_pair(key, value));
                }
            }

        private:
            std::map<std::shared_ptr<Node::Node>, std::shared_ptr<T>> m_managedMap;
        };
    }
}