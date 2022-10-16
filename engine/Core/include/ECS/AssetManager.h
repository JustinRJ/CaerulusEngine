#pragma once

#include <map>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

#include "ECS/IManager.h"
#include "Log/Log.h"

namespace Core
{
    namespace ECS
    {
        // TODO - clean this class up
        template <class R>
        class AssetManager : public IManager
        {
        public:
            AssetManager() :
                m_typeHash(typeid(R).hash_code())
            {}

            virtual ~AssetManager() = default;

            size_t GetManagedTypeHash() const override
            {
                return m_typeHash;
            }

            virtual void Insert(std::string_view key, const std::shared_ptr<R>& value)
            {
                if (value)
                {
                    m_managedMap.emplace(key.data(), value);
                }
            }

            virtual void Remove(std::string_view key)
            {
                auto it = m_managedMap.find(key.data());
                if (it != std::end(m_managedMap))
                {
                    m_managedMap.erase(it);
                }
            }

            std::shared_ptr<R> Get(std::string_view key) const
            {
                std::shared_ptr<R> found;
                auto it = m_managedMap.find(key.data());
                if (it != m_managedMap.end())
                {
                    found = it->second;
                }
                return found;
            }

        protected:
            std::map<const std::string, std::shared_ptr<R>>& GetMap()
            {
                return m_managedMap;
            }

            std::map<const std::string, std::shared_ptr<R>> m_managedMap;
        private:
            size_t m_typeHash;
        };
    }
}