#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "Core/Logging/Log.h"
#include "Core/Interface/NonCopyable.h"

namespace
{
    using namespace Core::Logging;
}

namespace Managers
{
    template <class T>
    class CAERULUS_MANAGERS Manager : public Core::Interface::NonCopyable
    {
    public:
        Manager() = default;
        virtual ~Manager() = default;

        std::shared_ptr<T> Get(const std::string& key) const
        {
            if (IsLoaded(key))
            {
                return m_managedMap.at(key);
            }
            Log::LogInDebug("Managed object " + key + " not found!");
            return nullptr;
        }

        std::vector<std::shared_ptr<T>> GetAll() const
        {
            std::vector<std::shared_ptr<T>> elements;
            std::transform(m_managedMap.begin(), m_managedMap.end(),
                std::back_inserter(elements), [](auto &kv) { return kv.second; });
            return elements;
        }

        const std::map<const std::string, std::shared_ptr<T>>& GetMap() const
        {
            return m_managedMap;
        }

        bool Remove(const std::string& key)
        {
            if (IsLoaded(key))
            {
                delete m_managedMap[key];
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

    protected:
        void Insert(const std::string& key, std::shared_ptr<T> value)
        {
            if (value)
            {
                m_managedMap.insert(std::make_pair(key, value));
            }
        }
        std::map<const std::string, std::shared_ptr<T>> m_managedMap;
    };
}