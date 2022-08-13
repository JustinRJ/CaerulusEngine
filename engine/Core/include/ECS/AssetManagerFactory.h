#pragma once

#include "ECS/AssetManager.h"

namespace Core
{
    namespace ECS
    {
        class AssetManagerFactory
        {
        public:
            AssetManagerFactory() = default;
            virtual ~AssetManagerFactory() = default;

            template<class T>
            AssetManager<T>* GetAssetManagerForType()
            {
                size_t assetHashToFind = typeid(T).hash_code();
                IManager* foundAssetManager = nullptr;

                auto it = std::begin(m_managers);
                while (!foundAssetManager && it != std::end(m_managers))
                {
                    if (assetHashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundAssetManager = it->get();
                    }
                    it++;
                }
                return static_cast<AssetManager<T>*>(foundAssetManager);
            }

            template<class T>
            AssetManager<T>* CreateAssetManagerForType()
            {
                IManager* foundAssetManager = GetAssetManagerForType<T>();
                if (!foundAssetManager)
                {
                    m_managers.push_back(std::make_unique<AssetManager<T>>());
                    foundAssetManager = m_managers.back().get();
                }
                return static_cast<AssetManager<T>*>(foundAssetManager);
            }

            bool AddAssetManagerForType(const std::shared_ptr<IManager>& manager)
            {
                bool found = std::any_of(std::begin(m_managers),
                    std::end(m_managers),
                    [manager](const std::shared_ptr<IManager>& imanager)
                {
                    return imanager->GetManagedTypeHash() == manager->GetManagedTypeHash();
                });

                if (!found)
                {
                    m_managers.push_back(manager);
                }
                return !found;
            }

        private:
            std::vector<std::shared_ptr<IManager>> m_managers;
        };
    }
}