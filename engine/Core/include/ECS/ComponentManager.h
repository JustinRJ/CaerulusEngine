#pragma once

#include "ECS/Manager.h"
#include "ECS/Component.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public Manager<ECS::Entity*, R>
        {
        public:
            ComponentManager()
            {
                ComponentManagerData data;
                data.ManagedTypeHash = Manager<ECS::Entity*, R>::GetManagedTypeHash();
                data.AddComponent = [&](Entity& e)
                {
                    auto newComponent = std::make_unique<R>(e);
                    auto returnNewComponent = newComponent.get();
                    Insert(&e, std::move(newComponent));
                    return returnNewComponent;
                };
                data.RemoveComponent = [&](Entity& e)
                {
                    auto& map = Manager<ECS::Entity*, R>::GetMap();
                    auto it = map.find(&e);
                    if (it != std::end(map))
                    {
                        auto& component = it->second;
                        component->OnDestroy();
                        map.erase(it);
                    }
                };

                Entity::RegisterComponentManager(data);
            }

            virtual ~ComponentManager() = default;

            void Remove(ECS::Entity* key) override
            {
                key->RemoveComponentOfType<R>();
            }

            void EarlyUpdate()
            {
                for (auto& [entity, component] : Manager<ECS::Entity*, R>::GetMap())
                {
                    if (component->IsEnabled())
                    {
                        component->EarlyUpdate();
                    }
                }
            }

            void Update(float deltaTime)
            {
                for (auto& [entity, component] : Manager<ECS::Entity*, R>::GetMap())
                {
                    if (component->IsEnabled())
                    {
                        component->Update(deltaTime);
                    }
                }
            }

            void FixedUpdate(float fixedTime)
            {
                for (auto& [entity, component] : Manager<ECS::Entity*, R>::GetMap())
                {
                    if (component->IsEnabled())
                    {
                        component->FixedUpdate(fixedTime);
                    }
                }
            }

            void LateUpdate()
            {
                for (auto& [entity, component] : Manager<ECS::Entity*, R>::GetMap())
                {
                    if (component->IsEnabled())
                    {
                        component->LateUpdate();
                    }
                }
            }

            void Reset()
            {
                for (auto& [entity, component] : Manager<ECS::Entity*, R>::GetMap())
                {
                    component->Reset();
                }
            }

        private:
            void Insert(ECS::Entity* entity, std::unique_ptr<R>&& value) override
            {
                if (entity && value)
                {
                    assert(dynamic_cast<ECS::Component*>(value.get()));

                    R* onAwakeAfterInsert = value.get();
                    Manager<ECS::Entity*, R>::Insert(entity, std::move(value));
                    onAwakeAfterInsert->OnAwake();
                }
            }
        };
    }
}