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
        class Manager : Interface::NonCopyable
        {
        public:
            Manager() = default;
            virtual ~Manager() = default;

            const T* Get(const std::string& key) const
            {
                return GetMutable(key);
            }

            const std::map<const std::string, std::unique_ptr<T>>& GetMap() const
            {
                return m_managedMap;
            }

            bool Remove(const std::string& key)
            {
                if (IsLoaded(key))
                {
                    return m_managedMap.erase(key) > 0;
                }
                return false;
            }

            bool IsLoaded(const std::string& key) const
            {
                return m_managedMap.find(key) != m_managedMap.end();
            }

            void Clear()
            {
                m_managedMap.clear();
            }

            void Insert(const std::string& key, std::unique_ptr<T>&& value)
            {
                m_managedMap.insert(std::make_pair(key, std::move(value)));
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

            // TODO - add map for mask - T pair

            // TODO - add map for void pointer - T pair
        };
    }
}