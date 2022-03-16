#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class PointLight : public Light
        {
        public:
            PointLight(Core::Node::Node& node) :
                Light(node)
            {}

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