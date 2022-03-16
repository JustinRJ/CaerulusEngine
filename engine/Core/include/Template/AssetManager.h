#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "Logging/Log.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace Template
    {
        template <class T>
        class AssetManager : Interface::NonCopyable
        {
        public:
            AssetManager() = default;
            virtual ~AssetManager() = default;

            const T* Get(const std::string& key) const
            {
                return GetMutable(key);
            }

            std::vector<const T*> GetAll() const
            {
                std::vector<const T*> elements;
                std::transform(std::begin(m_managedMap), std::end(m_managedMap),
                    std::back_inserter(elements), [](auto& kv) { return dynamic_cast<T*>(kv.second.get()); });
                return elements;
            }

            bool Remove(const std::string& key)
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

            bool IsLoaded(const std::string& key) const
            {
                return m_managedMap.find(key) != m_managedMap.end();
            }

            void Insert(const std::string& key, std::unique_ptr<T>&& value)
            {
                if (!key.empty() && value)
                {
                    m_managedMap.emplace(key, std::move(value));
                }
            }

            std::unique_ptr<T> Release(const std::string& key)
            {
                std::unique_ptr<T> released = nullptr;

                auto it = m_managedMap.find(key);
                if (it != m_managedMap.end())
                {
                    released = std::move(it->second);
                    m_managedMap.erase(it);
                }

                return released;
            }

        protected:
            T* GetMutable(const std::string& key) const
            {
                auto it = m_managedMap.find(key);
                if (it != m_managedMap.end())
                {
                    return it->second.get();
                }
                Logging::LogInDebug("Managed object " + key + " not found!");
                return nullptr;
            }

        private:
            std::map<const std::string, std::unique_ptr<T>> m_managedMap;
        };
    }
}