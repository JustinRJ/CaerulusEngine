#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class PointLight : public Light
        {
        public:
            PointLight() = default;
            ~PointLight() = default;

            float GetRadius() const
            {
                return m_radius;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
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
            float m_radius;
        };
    }
}