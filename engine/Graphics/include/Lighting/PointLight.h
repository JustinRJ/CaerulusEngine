#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Lighting
    {
        class PointLight : public Light
        {
        public:
            PointLight(Core::Node::Node* parent, const Managers::ShaderManager& shaderManager) :
                Light(parent, shaderManager)
            {}

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
            Core::Math::vec3 m_position = Core::Math::vec3(0, 0, 0);
            float m_radius = 1.f;
        };
    }
}