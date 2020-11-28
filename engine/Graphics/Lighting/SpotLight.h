#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class SpotLight : public Light
        {
        public:
            SpotLight() = default;
            ~SpotLight() = default;

            void GetAngle(float angle)
            {
                m_angle = angle;
            }

            float SetAngle() const
            {
                return m_angle;
            }

            const Core::Math::vec3& GetPosition() const
            {
                return m_position;
            }

            void SetPosition(const Core::Math::vec3& pos)
            {
                m_position = pos;
            }

        private:
            Core::Math::vec3 m_position;
            float m_angle;
        };
    }
}