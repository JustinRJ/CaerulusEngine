#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class PointLight : public Light
        {
        public:
            void Tick(float deltaTime)
            {
                InvokeUniformCallbacks();
            }

            float GetRadius() const
            {
                return m_radius;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
            }

        private:
            float m_radius = 1.f;
        };
    }
}