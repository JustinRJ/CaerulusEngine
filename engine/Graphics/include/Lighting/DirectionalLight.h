#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class DirectionalLight : public Light
        {
        public:
            DirectionalLight(Core::ECS::Entity& entity) :
                Light(entity),
                m_direction(0,0,0)
            {}

            const Core::Math::vec3& GetDirection() const
            {
                return m_direction;
            }

            void SetDirection(const Core::Math::vec3& direction)
            {
                m_direction = direction;
            }

        private:
            Core::Math::vec3 m_direction;
        };
    }
}