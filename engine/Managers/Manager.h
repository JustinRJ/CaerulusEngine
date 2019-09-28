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

        virtual T* Get(const std::string& key) const
        {
            if (IsLoaded(key))
            {
                return m_ManagedMap.at(key);
            }
            std::cerr << "Managed object " + key + " not found!" << std::endl;
            return nullptr;
        }

        virtual std::vector<T*> GetAll(const std::vector<std::string>& keys) const
        {
            std::vector<T*> values = std::vector<T*>();
            for (unsigned int i = 0; i < keys.size(); i++)
            {
                values.push_back(Get(keys.at(i)));
            }
            return values;
        }

        virtual bool Remove(const std::string& key)
        {
            delete m_ManagedMap.at(key);
            return m_ManagedMap.erase(key) > 0;
        }

        virtual bool IsLoaded(const std::string& key) const
        {
            if (m_ManagedMap.find(key) != m_ManagedMap.end())
            {
                return true;
            }
            return false;
        }

    protected:

        virtual void Insert(const std::string& key, T* value)
        {
            if (value)
            {
                m_ManagedMap.insert(std::make_pair(key, value));
            }
        }
#pragma warning(push)
#pragma warning( disable : 4251)
        std::map<const std::string, T*> m_ManagedMap;
#pragma warning(pop)
    };
}