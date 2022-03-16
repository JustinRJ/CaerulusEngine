#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "Logging/Log.h"
#include "Interface/NonCopyable.h"
#include "ECS/ThreadSafe.h"

namespace Core
{
    namespace ECS
    {
        template <class T, class R>
        class Manager : Interface::NonCopyable
        {
        public:
            Manager() = default;
            virtual ~Manager() = default;

            virtual void Insert(T key, std::unique_ptr<R>&& value)
            {
                if (value)
                {
                    m_managedMap.emplace(key, std::move(value));
                }
            }

#ifdef THREAD_SAFE
            const R* Get(T key) const
#endif
#ifdef THREAD_UNSAFE
            R* Get(T key) const
#endif
            {
                return GetMutable(key);
            }

            std::vector<const R*> GetAll() const
            {
                std::vector<const R*> elements(m_managedMap.size());
                std::transform(std::begin(m_managedMap), std::end(m_managedMap),
                    std::begin(elements), [](auto& kv) { return static_cast<R*>(kv.second.get()); });
                return elements;
            }

            bool Remove(T key)
            {
                bool removed = false;
                auto it = m_managedMap.find(key);
                if (it != std::end(m_managedMap))
                {
                    m_managedMap.erase(it);
                    removed = true;
                }
                return removed;
            }

            std::unique_ptr<R> Release(T key)
            {
                std::unique_ptr<R> released = nullptr;

                auto it = m_managedMap.find(key);
                if (it != m_managedMap.end())
                {
                    released = std::move(it->second);
                    m_managedMap.erase(it);
                }

                return released;
            }

        protected:
            R* GetMutable(T key) const
            {
                R* found = nullptr;
                auto it = m_managedMap.find(key);
                if (it != m_managedMap.end())
                {
                    found = it->second.get();
                }
                return found;
            }

            const std::map<T, std::unique_ptr<R>>& GetMap() const
            {
                return m_managedMap;
            }

            std::map<T, std::unique_ptr<R>> m_managedMap;
        };
    }
}