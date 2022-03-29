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

                auto it = std::begin(m_assetManagers);
                while (!foundAssetManager && it != std::end(m_assetManagers))
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
                    m_assetManagers.push_back(std::make_unique<AssetManager<T>>());
                    foundAssetManager = m_assetManagers.back().get();
                }
                return static_cast<AssetManager<T>*>(foundAssetManager);
            }


        private:
            std::vector<std::unique_ptr<IManager>> m_assetManagers;
        };
    }
}