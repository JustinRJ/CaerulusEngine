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
                IManager* foundManager = nullptr;

                auto it = std::begin(m_managers);
                while (!foundManager && it != std::end(m_managers))
                {
                    if (componentHashToFind == (*it)->GetManagedTypeHash())
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
                    m_managers.push_back(std::make_shared<ComponentManager<T>>());
                    foundManager = m_managers.back().get();
                }
                return static_cast<ComponentManager<T>*>(foundManager);
            }

            bool AddComponentManagerForType(const std::shared_ptr<IManager>& manager)
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