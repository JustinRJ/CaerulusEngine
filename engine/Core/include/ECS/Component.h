#pragma once

#include "ECS/Entity.h"
#include "Interface/NonCopyable.h"

namespace Core
{
    namespace ECS
    {
        class CAERULUS_CORE Component : Interface::NonCopyable
        {
        public:
            Component(Entity& entity) :
                m_entity(entity)
            {}

            virtual ~Component() = default;

            virtual void OnAwake() {};
            virtual void OnDestroy() {};

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
            
            bool IsEnabled() const
            {
                return m_isEnabled && m_entity.IsEnabled();
            }

            void SetEnabled(bool enabled)
            {
                if (m_entity.IsEnabled())
                {
                    if (m_isEnabled != enabled)
                    {
                        m_isEnabled = enabled;
                        enabled ? OnEnable() : OnDisable();
                    }
                }
                else if (m_isEnabled != false)
                {
                    m_isEnabled = false;
                    OnDisable();
                }
            }

        protected:
            Entity& m_entity;

        private:
            bool m_isEnabled = true;
        };
    }
}
