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
            AssetManager<T>* GetAssetManagerForType()
            {
                size_t hashToFind = typeid(T).hash_code();
                IManager* foundManager = nullptr;

                auto it = std::begin(m_assetManagers);
                while (!foundManager && it != std::end(m_assetManagers))
                {
                    if (hashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundManager = it->get();
                    }
                    it++;
                }
                return static_cast<AssetManager<T>*>(foundManager);
            }

            template<class T>
            AssetManager<T>* CreateAssetManagerForType()
            {
                IManager* foundManager = GetAssetManagerForType<T>();
                if (!foundManager)
                {
                    m_assetManagers.push_back(std::make_unique<AssetManager<T>>());
                    foundManager = m_assetManagers.back().get();
                }
                return static_cast<AssetManager<T>*>(foundManager);
            }

            bool AddAssetManagerForType(const std::shared_ptr<IManager>& manager)
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
            ComponentManager<T>* GetComponentManagerForType()
            {
                size_t hashToFind = typeid(T).hash_code();
                IManager* foundManager = nullptr;

                auto it = std::begin(m_componentManagers);
                while (!foundManager && it != std::end(m_componentManagers))
                {
                    if (hashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundManager = it->get();
                    }
                    it++;
                }
                return static_cast<ComponentManager<T>*>(foundManager);
            }

            template<class T>
            ComponentManager<T>* CreateComponentManagerForType()
            {
                IManager* foundManager = GetComponentManagerForType<T>();
                if (!foundManager)
                {
                    m_componentManagers.push_back(std::make_shared<ComponentManager<T>>());
                    foundManager = m_componentManagers.back().get();
                }
                return static_cast<ComponentManager<T>*>(foundManager);
            }

            bool AddComponentManagerForType(const std::shared_ptr<IManager>& manager)
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