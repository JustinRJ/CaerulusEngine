#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace Managers
{
    template <class T>
    class CAERULUS_MANAGERS Manager
    {
    public:

        Manager() {}
        virtual ~Manager() {}

        T* Get(const std::string& key) const
        {
            if (IsLoaded(key))
            {
                return m_ManagedMap.at(key);
            }
            std::cerr << "Managed object " + key + " not found!" << std::endl;
            return nullptr;
        }

        std::vector<T*> GetAll(const std::vector<std::string>& keys) const
        {
            std::vector<T*> values = std::vector<T*>();
            for (unsigned int i = 0; i < keys.size(); i++)
            {
                values.push_back(Get(keys.at(i)));
            }
            return values;
        }

        bool Remove(const std::string& key)
        {
            delete m_ManagedMap.at(key);
            return m_ManagedMap.erase(key) > 0;
        }

        bool IsLoaded(const std::string& key) const
        {
            if (m_ManagedMap.find(key) != m_ManagedMap.end())
            {
                return true;
            }
            return false;
        }

        const std::map<const std::string, T*>& GetMap() const
        {
            return m_ManagedMap;
        }

    protected:

        void Insert(const std::string& key, T* value)
        {
            if (value)
            {
                m_ManagedMap.insert(std::make_pair(key, value));
            }
        }
        std::map<const std::string, T*> m_ManagedMap;
    };
}