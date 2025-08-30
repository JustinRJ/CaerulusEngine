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
            std::shared_ptr<AssetManager<T>> GetAssetManager()
            {
                size_t hash = typeid(T).hash_code();
                auto it = m_assetManagers.find(hash);
                if (it != m_assetManagers.end())
                {
                    return std::static_pointer_cast<AssetManager<T>>(it->second);
                }
                return nullptr;
            }

            template<class T>
            std::shared_ptr<AssetManager<T>> CreateAssetManager()
            {
                auto manager = GetAssetManager<T>();
                if (!manager)
                {
                    auto newManager = std::make_shared<AssetManager<T>>();
                    m_assetManagers[typeid(T).hash_code()] = newManager;
                    manager = newManager;
                }
                return manager;
            }

            bool AddAssetManager(const std::shared_ptr<IAssetManager>& manager)
            {
                size_t hash = manager->GetHashCode();
                auto [it, inserted] = m_assetManagers.emplace(hash, manager);
                return inserted;
            }

            template<class T>
            std::shared_ptr<ComponentManager<T>> GetComponentManager()
            {
                size_t hash = typeid(T).hash_code();
                auto it = m_componentManagers.find(hash);
                if (it != m_componentManagers.end())
                {
                    return std::static_pointer_cast<ComponentManager<T>>(it->second);
                }
                return nullptr;
            }

            template<class T>
            std::shared_ptr<ComponentManager<T>> CreateComponentManager(EntityManager& em)
            {
                auto manager = GetComponentManager<T>();
                if (!manager)
                {
                    auto newManager = std::make_shared<ComponentManager<T>>(em);
                    m_componentManagers[typeid(T).hash_code()] = newManager;
                    manager = newManager;
                }
                return manager;
            }

            bool AddComponentManager(const std::shared_ptr<IComponentManager>& manager)
            {
                size_t hash = manager->GetHashCode();
                auto [it, inserted] = m_componentManagers.emplace(hash, manager);
                return inserted;
            }

        private:
            std::unordered_map<size_t, std::shared_ptr<IAssetManager>> m_assetManagers;
            std::unordered_map<size_t, std::shared_ptr<IComponentManager>> m_componentManagers;
        };

    }
}