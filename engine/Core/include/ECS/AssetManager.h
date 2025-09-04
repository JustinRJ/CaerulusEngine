#pragma once

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <unordered_map>

#include "Log/Log.h"
#include "ECS/IAssetManager.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class AssetManager : public IAssetManager
        {
        public:
            AssetManager()
            {}

            virtual ~AssetManager() = default;

            constexpr size_t GetHashCode() const override
            {
                return typeid(R).hash_code();
            }

            virtual void Insert(std::string_view key, std::unique_ptr<R>&& value)
            {
                if (value)
                {
                    m_managedMap.emplace(key.data(), std::move(value));
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

            R* Get(std::string_view key) const
            {
                R* found = nullptr;
                auto it = m_managedMap.find(key.data());
                if (it != m_managedMap.end())
                {
                    found = it->second.get();
                }
                return found;
            }

        private:
            std::unordered_map<std::string, std::unique_ptr<R>> m_managedMap;
        };
    }
}