#pragma once

#include "ECS/AssetManager.h"

namespace Core
{
    namespace ECS
    {
        // TODO - improve this class
        class AssetManagerFactory
        {
        public:
            AssetManagerFactory() = default;
            virtual ~AssetManagerFactory() = default;

            template<class T>
            AssetManager<T>* GetAssetManager()
            {
                size_t hash = typeid(T).hash_code();
                auto it = m_assetManagers.find(hash);
                if (it != m_assetManagers.end())
                {
                    return static_cast<AssetManager<T>*>(it->second.get());
                }
                return nullptr;
            }

            template<class T>
            AssetManager<T>* CreateAssetManager()
            {
                auto manager = GetAssetManager<T>();
                if (!manager)
                {
                    auto newManager = std::make_unique<AssetManager<T>>();
                    manager = newManager.get();
                    m_assetManagers[typeid(T).hash_code()] = std::move(newManager);
                }
                return manager;
            }

            bool AddAssetManager(std::unique_ptr<Interface::IHashable>&& manager)
            {
                size_t hash = manager->GetHashCode();
                auto [it, inserted] = m_assetManagers.emplace(hash, std::move(manager));
                return inserted;
            }

        private:
            std::unordered_map<size_t, std::unique_ptr<Interface::IHashable>> m_assetManagers;
        };

    }
}