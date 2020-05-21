#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include <map>
#include <vector>
#include <string>
#include "../../Core/Logging/Log.h"
#include "../../Core/Interface/NonCopyable.h"

namespace Managers
{
    template <class T>
    class CAERULUS_MANAGERS Manager : public Core::Interface::NonCopyable
    {
    public:

        Manager() {}
        virtual ~Manager() = default;

        std::shared_ptr<T> Get(const std::string& key) const
        {
            using namespace Core::Logging;
            if (IsLoaded(key))
            {
                return m_managedMap.at(key);
            }
            Log::LogInDebug("Managed object " + key + " not found!");
            return nullptr;
        }

        bool Remove(const std::string& key)
        {
            delete m_managedMap.at(key);
            return m_managedMap.erase(key) > 0;
        }

        bool IsLoaded(const std::string& key) const
        {
            if (m_managedMap.find(key) != m_managedMap.end())
            {
                return true;
            }
            return false;
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