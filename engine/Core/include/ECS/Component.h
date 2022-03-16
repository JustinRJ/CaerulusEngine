#pragma once

#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class Entity;

        class CAERULUS_CORE Component : Interface::NonCopyable
        {
        public:
            Component(Entity& entity) :
                m_entity(entity)
            {}

            virtual ~Component() = default;

            virtual void OnEnable() {};
            virtual void OnDisable() {};

            virtual void EarlyUpdate() {};
            virtual void Update(float deltaTime) {};
            virtual void FixedUpdate(float fixedTime) {};
            virtual void LateUpdate() {};

            virtual void Reset() {};

            Entity& GetEntity()
            {
                return m_entity;
            }

            const Entity& GetEntity() const
            {
                return m_entity;
            }

        protected:
            Entity& m_entity;
        };
    }
}
