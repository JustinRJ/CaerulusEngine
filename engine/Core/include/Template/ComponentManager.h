#pragma once

#include <map>
#include <memory>
#include <vector>
#include <algorithm>

#include "Node/Node.h"
#include "Logging/Log.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Template
    {
        // todo create manager class that asset and component derive from
        template <class T>
        class ComponentManager : Interface::NonCopyable
        {
        public:
            ComponentManager() = default;
            virtual ~ComponentManager() = default;

            const T* Get(const Node::Node& key)
            {
                return GetMutable(key);
            }

            std::vector<const T*> GetAll() const
            {
                std::vector<const T*> elements;
                std::transform(std::begin(m_managedMap), std::end(m_managedMap),
                    std::back_inserter(elements), [](auto &kv) { return dynamic_cast<T*>(kv.second.get()); });
                return elements;
            }

            bool Remove(const Node::Node& key)
            {
                bool removed = false;
                auto it = m_managedMap.find(&key);
                if (it != std::end(m_managedMap))
                {
                    m_managedMap.erase(it);
                    removed = true;
                }
                return removed;
            }

            bool IsLoaded(const Node::Node& key) const
            {
                return m_managedMap.find(&key) != m_managedMap.end();
            }

            void Insert(std::unique_ptr<T>&& value)
            {
                // static_assert(dynamic_cast<Node::Component*>(value.get());

                if (value)
                {
                    Core::Node::Node* key = &value->GetNode();
                    Node::ComponentData data({ value.get(), [&](const Node::Node& nodeToRemove) { Remove(nodeToRemove); } });
                    key->AddComponent(data);
                    m_managedMap.emplace(key, std::move(value));
                }
            }

            void EarlyUpdate()
            {
                for (auto it = std::begin(m_managedMap); it != std::end(m_managedMap); it++)
                {
                    it->second.get()->EarlyUpdate();
                }
            }

            void Update(float deltaTime)
            {
                for (auto it = std::begin(m_managedMap); it != std::end(m_managedMap); it++)
                {
                    it->second.get()->Update(deltaTime);
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto it = std::begin(m_managedMap); it != std::end(m_managedMap); it++)
                {
                    it->second.get()->FixedUpdate(fixedTime);
                }
            }

            void LateUpdate()
            {
                for (auto it = std::begin(m_managedMap); it != std::end(m_managedMap); it++)
                {
                    it->second.get()->LateUpdate();
                }
            }

            void Reset()
            {
                for (auto it = std::begin(m_managedMap); it != std::end(m_managedMap); it++)
                {
                    it->second.get()->Reset();
                }
            }
        protected:
            T* GetMutable(const Node::Node& key)
            {
                auto it = m_managedMap.find(&key);
                if (it != m_managedMap.end())
                {
                    return it->second.get();
                }
                Logging::LogInDebug("Managed node object with ID" + std::to_string(key.GetID()) + " not found!");
                return nullptr;
            }
        private:
            std::map<const Node::Node*, std::unique_ptr<T>> m_managedMap;
        };
    }
}