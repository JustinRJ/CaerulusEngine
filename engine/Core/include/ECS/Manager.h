#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "ECS/IManager.h"
#include "ECS/ThreadSafe.h"
#include "Logging/Log.h"

namespace Core
{
    namespace ECS
    {
        template <class T, class R>
        class Manager : public IManager
        {
        public:
            Manager() :
                m_typeHash(typeid(R).hash_code())
            {}

            virtual ~Manager() = default;

            size_t GetManagedTypeHash() const override
            {
                return m_typeHash;
            }

            virtual void Insert(T key, std::unique_ptr<R>&& value)
            {
                if (value)
                {
                    m_managedMap.emplace(key, std::move(value));
                }
            }

            virtual void Remove(T key)
            {
                auto it = m_managedMap.find(key);
                if (it != std::end(m_managedMap))
                {
                    m_managedMap.erase(it);
                }
            }

#ifdef THREAD_SAFE
            const R* Get(T key) const
#else
            R* Get(T key) const
#endif
            {
                return GetMutable(key);
            }

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

            std::vector<const R*> GetAll() const
            {
                std::vector<const R*> elements(m_managedMap.size());
                std::transform(std::begin(m_managedMap), std::end(m_managedMap),
                    std::begin(elements), [](auto& kv) { return static_cast<R*>(kv.second.get()); });
                return elements;
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
            std::map<T, std::unique_ptr<R>>& GetMap()
            {
                return m_managedMap;
            }

        private:
            size_t m_typeHash;
            std::map<T, std::unique_ptr<R>> m_managedMap;
        };
    }
}