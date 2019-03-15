#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace Managers
{
    template <class T>
    class Manager
    {
    public:

        CAERULUS_MANAGERS Manager() {}

        CAERULUS_MANAGERS virtual ~Manager() {}

        CAERULUS_MANAGERS virtual T* Get(const std::string& key) const
        {
            if (IsLoaded(key))
            {
                return m_ManagedMap.at(key);
            }
            std::cerr << "Managed object " + key + " not found!" << std::endl;
            return nullptr;
        }

        CAERULUS_MANAGERS virtual std::vector<T*> GetAll(const std::vector<std::string>& keys) const
        {
            std::vector<T*> values = std::vector<T*>();
            for (unsigned int i = 0; i < keys.size(); i++)
            {
                values.push_back(Get(keys.at(i)));
            }
            return values;
        }

        CAERULUS_MANAGERS virtual bool Remove(const std::string& key)
        {
            delete m_ManagedMap.at(key);
            return m_ManagedMap.erase(key) > 0;
        }

        CAERULUS_MANAGERS virtual bool IsLoaded(const std::string& key) const
        {
            if (m_ManagedMap.find(key) != m_ManagedMap.end())
            {
                return true;
            }
            return false;
        }

    protected:

        CAERULUS_MANAGERS virtual void Insert(const std::string& key, T* value)
        {
            if (value)
            {
                m_ManagedMap.insert(std::make_pair(key, value));
            }
        }

        std::map<const std::string, T*> m_ManagedMap;
    };
}