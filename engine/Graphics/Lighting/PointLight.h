#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light
        {
        public:
            PointLight(std::shared_ptr<Pipeline::Shader> shader) :
                Light(shader)
            {}

            virtual ~PointLight() = default;

            const Core::Math::vec3& GetPosition() const
            {
                return m_position;
            }

            float GetRadius() const
            {
                return m_radius;
            }

            void SetPosition(const Core::Math::vec3& pos)
            {
                m_position = pos;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
            }

        private:
            Core::Math::vec3 m_position;
            float m_radius;
        };
    }
}