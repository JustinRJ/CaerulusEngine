#pragma once

#include "ECS/Component.h"
#include "ECS/Manager.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public Manager<ECS::Entity*, R>
        {
        public:
            ComponentManager() = default;
            virtual ~ComponentManager() = default;

            void Insert(ECS::Entity* entity, std::unique_ptr<R>&& value) override
            {
                if (entity && value)
                {
                    assert(dynamic_cast<ECS::Component*>(value.get()));

                    ComponentData data;
                    data.Component = value.get();
                    data.ComponentHash = GetManagedObjectHash();
                    data.RemoveComponent = [&](Entity& e)
                    {
                        auto& map = Manager<ECS::Entity*, R>::GetMap();
                        auto it = map.find(&e);
                        if (it != std::end(map))
                        {
                            it->second->OnDestroy();
                            map.erase(it);
                        }
                    };

                    entity->AddComponent(data);

                    R* onAwakeAfterInsert = value.get();
                    Manager<ECS::Entity*, R>::Insert(entity, std::move(value));
                    onAwakeAfterInsert->OnAwake();
                }
            }

            void Remove(ECS::Entity* key) override
            {
                key->RemoveComponentOfType<R>();
            }

            size_t GetManagedObjectHash() const
            {
                return typeid(R).hash_code();
            }

            void EarlyUpdate()
            {
                const auto& map = Manager<ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->EarlyUpdate();
                }
            }

            void Update(float deltaTime)
            {
                const auto& map = Manager<ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->Update(deltaTime);
                }
            }

            void FixedUpdate(float fixedTime)
            {
                const auto& map = Manager<ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->FixedUpdate(fixedTime);
                }
            }

            void LateUpdate()
            {
                const auto& map = Manager<ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->LateUpdate();
                }
            }

            void Reset()
            {
                const auto& map = Manager<ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->Reset();
                }
            }
        };
    }
}