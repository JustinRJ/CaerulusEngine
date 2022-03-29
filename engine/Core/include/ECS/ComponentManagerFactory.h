#pragma once

#include "ECS/ComponentManager.h"

namespace Core
{
    namespace ECS
    {
        class ComponentManagerFactory
        {
        public:
            ComponentManagerFactory() = default;
            virtual ~ComponentManagerFactory() = default;

            template<class T>
            ComponentManager<T>* GetComponentManagerForType()
            {
                size_t componentHashToFind = typeid(T).hash_code();
                IManager* foundComponentManager = nullptr;

                auto it = std::begin(m_componentManagers);
                while (!foundComponentManager && it != std::end(m_componentManagers))
                {
                    if (componentHashToFind == (*it)->GetManagedTypeHash())
                    {
                        foundComponentManager = it->get();
                    }
                    it++;
                }
                return static_cast<ComponentManager<T>*>(foundComponentManager);
            }

            template<class T>
            ComponentManager<T>* CreateComponentManagerForType()
            {
                IManager* foundComponentManager = GetComponentManagerForType<T>();
                if (!foundComponentManager)
                {
                    m_componentManagers.push_back(std::make_unique<ComponentManager<T>>());
                    foundComponentManager = m_componentManagers.back().get();
                }
                return static_cast<ComponentManager<T>*>(foundComponentManager);
            }


        private:
            std::vector<std::unique_ptr<IManager>> m_componentManagers;
        };
    }
}