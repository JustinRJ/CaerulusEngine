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
                size_t hashToFind = typeid(T).hash_code();
                std::shared_ptr<IManager> foundManager = nullptr;
                auto it = std::begin(m_assetManagers);
                while (!foundManager && it != std::end(m_assetManagers))
                {
                    if (hashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundManager = *it;
                    }
                    it++;
                }
                return std::static_pointer_cast<AssetManager<T>>(foundManager);
            }

            template<class T, class U>
            std::shared_ptr<U> GetAssetManagerAsType()
            {
                return std::static_pointer_cast<U>(GetAssetManager<T>());
            }

            template<class T>
            std::shared_ptr<AssetManager<T>> CreateAssetManager()
            {
                std::shared_ptr<IManager> foundManager = GetAssetManager<T>();
                if (!foundManager)
                {
                    m_assetManagers.push_back(std::make_unique<AssetManager<T>>());
                    foundManager = m_assetManagers.back();
                }
                return std::static_pointer_cast<AssetManager<T>>(foundManager);
            }

            bool AddAssetManager(const std::shared_ptr<IManager>& manager)
            {
                bool found = std::any_of(std::begin(m_assetManagers),
                    std::end(m_assetManagers),
                    [manager](const std::shared_ptr<IManager>& imanager)
                    {
                        return imanager->GetManagedTypeHash() == manager->GetManagedTypeHash();
                    });

                if (!found)
                {
                    m_assetManagers.push_back(manager);
                }
                return !found;
            }

            template<class T>
            std::shared_ptr<ComponentManager<T>> GetComponentManager()
            {
                size_t hashToFind = typeid(T).hash_code();
                std::shared_ptr<IManager> foundManager = nullptr;

                auto it = std::begin(m_componentManagers);
                while (!foundManager && it != std::end(m_componentManagers))
                {
                    if (hashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundManager = *it;
                    }
                    it++;
                }
                return std::static_pointer_cast<ComponentManager<T>>(foundManager);
            }

            template<class T, class U>
            std::shared_ptr<ComponentManager<T>> GetComponentManagerAsType()
            {
                return std::static_pointer_cast<U>(GetComponentManager<T>());
            }

            template<class T>
            std::shared_ptr<ComponentManager<T>> CreateComponentManager()
            {
                std::shared_ptr<IManager> foundManager = GetComponentManager<T>();
                if (!foundManager)
                {
                    m_componentManagers.push_back(std::make_shared<ComponentManager<T>>());
                    foundManager = m_componentManagers.back();
                }
                return std::static_pointer_cast<ComponentManager<T>>(foundManager);
            }

            bool AddComponentManager(const std::shared_ptr<IManager>& manager)
            {
                bool found = std::any_of(std::begin(m_componentManagers),
                    std::end(m_componentManagers),
                    [manager](const std::shared_ptr<IManager>& imanager)
                    {
                        return imanager->GetManagedTypeHash() == manager->GetManagedTypeHash();
                    });

                if (!found)
                {
                    m_componentManagers.push_back(manager);
                }
                return !found;
            }

        private:
            std::vector<std::shared_ptr<IManager>> m_assetManagers;
            std::vector<std::shared_ptr<IManager>> m_componentManagers;
        };
    }
}