#pragma once

#include "ECS/AssetManager.h"
#include "ECS/ComponentManager.h"

namespace Core
{
    namespace ECS
    {
        class ManagerFactory
        {
        public:
            ManagerFactory() = default;
            virtual ~ManagerFactory() = default;

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

            bool AddAssetManager(std::unique_ptr<IAssetManager>&& manager)
            {
                size_t hash = manager->GetHashCode();
                auto [it, inserted] = m_assetManagers.emplace(hash, std::move(manager));
                return inserted;
            }

            template<class T>
            ComponentManager<T>* GetComponentManager()
            {
                size_t hash = typeid(T).hash_code();
                auto it = m_componentManagers.find(hash);
                if (it != m_componentManagers.end())
                {
                    return static_cast<ComponentManager<T>*>(it->second.get());
                }
                return nullptr;
            }

            template<class T>
            ComponentManager<T>* CreateComponentManager(EntityManager& em)
            {
                auto manager = GetComponentManager<T>();
                if (!manager)
                {
                    auto newManager = std::make_unique<ComponentManager<T>>(em);
                    manager = newManager.get();
                    m_componentManagers[typeid(T).hash_code()] = std::move(newManager);
                }
                return manager;
            }

            bool AddComponentManager(std::unique_ptr<IComponentManager>&& manager)
            {
                size_t hash = manager->GetHashCode();
                auto [it, inserted] = m_componentManagers.emplace(hash, std::move(manager));
                return inserted;
            }

        private:
            std::unordered_map<size_t, std::unique_ptr<IAssetManager>> m_assetManagers;
            std::unordered_map<size_t, std::unique_ptr<IComponentManager>> m_componentManagers;
        };

    }
}