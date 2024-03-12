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
                std::shared_ptr<IHashable> foundManager = nullptr;
                auto it = std::begin(m_assetManagers);
                while (!foundManager && it != std::end(m_assetManagers))
                {
                    if (hashToFind == (*it)->GetHashCode())
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
                std::shared_ptr<IHashable> foundManager = GetAssetManager<T>();
                if (!foundManager)
                {
                    m_assetManagers.push_back(std::make_unique<AssetManager<T>>());
                    foundManager = m_assetManagers.back();
                }
                return std::static_pointer_cast<AssetManager<T>>(foundManager);
            }

            bool AddAssetManager(const std::shared_ptr<IHashable>& manager)
            {
                bool found = std::any_of(std::begin(m_assetManagers),
                    std::end(m_assetManagers),
                    [manager](const std::shared_ptr<IHashable>& imanager)
                    {
                        return imanager->GetHashCode() == manager->GetHashCode();
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
                std::shared_ptr<IComponentManager> foundManager = nullptr;

                auto it = std::begin(m_componentManagers);
                while (!foundManager && it != std::end(m_componentManagers))
                {
                    if (hashToFind == (*it)->GetHashCode())
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
            std::shared_ptr<ComponentManager<T>> CreateComponentManager(uint32_t maxComponents)
            {
                std::shared_ptr<IComponentManager> foundManager = GetComponentManager<T>();
                if (!foundManager)
                {
                    m_componentManagers.push_back(std::make_shared<ComponentManager<T>>(maxComponents));
                    foundManager = m_componentManagers.back();
                }
                return std::static_pointer_cast<ComponentManager<T>>(foundManager);
            }

            bool AddComponentManager(const std::shared_ptr<IComponentManager>& manager)
            {
                bool found = std::any_of(std::begin(m_componentManagers),
                    std::end(m_componentManagers),
                    [manager](const std::shared_ptr<IComponentManager>& imanager)
                    {
                        return imanager->GetHashCode() == manager->GetHashCode();
                    });

                if (!found)
                {
                    m_componentManagers.push_back(manager);
                }
                return !found;
            }

        private:
            std::vector<std::shared_ptr<IHashable>> m_assetManagers;
            std::vector<std::shared_ptr<IComponentManager>> m_componentManagers;
        };
    }
}