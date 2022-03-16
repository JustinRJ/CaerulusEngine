#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class SpotLight : public Light
        {
        public:
            SpotLight(Core::ECS::Entity& entity) :
                Light(entity),
                m_angle(0)
            {}

            void GetAngle(float angle)
            {
                m_angle = angle;
            }

            float SetAngle() const
            {
                return m_angle;
            }

        private:
            float m_angle;
        };
    }
}