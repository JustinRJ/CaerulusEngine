#pragma once

#include "ECS/Entity.h"
#include "ECS/Manager.h"

namespace Core
{
    namespace ECS
    {
        template <class R>
        class ComponentManager : public Manager<const ECS::Entity*, R>
        {
        public:
            ComponentManager() = default;
            virtual ~ComponentManager() = default;

            void Insert(const ECS::Entity* entity, std::unique_ptr<R>&& value) override
            {
                if (entity && value)
                {
                    assert(dynamic_cast<ECS::Component*>(value.get()));
                    Manager<const ECS::Entity*, R>::Insert(entity, std::move(value));
                }
            }

            void EarlyUpdate()
            {
                const auto& map = Manager<const ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->EarlyUpdate();
                }
            }

            void Update(float deltaTime)
            {
                const auto& map = Manager<const ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->Update(deltaTime);
                }
            }

            void FixedUpdate(float fixedTime)
            {
                const auto& map = Manager<const ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->FixedUpdate(fixedTime);
                }
            }

            void LateUpdate()
            {
                const auto& map = Manager<const ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->LateUpdate();
                }
            }

            void Reset()
            {
                const auto& map = Manager<const ECS::Entity*, R>::GetMap();
                for (auto it = std::begin(map); it != std::end(map); it++)
                {
                    it->second->Reset();
                }
            }
        };
    }
}