#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class PointLight : public Light
        {
        public:
            PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
                Light(shader, camera)
            {}

            virtual ~PointLight() = default;

            const vec3& GetPosition() const
            {
                return m_position;
            }

            float GetRadius() const
            {
                return m_radius;
            }

            void SetPosition(const vec3& pos)
            {
                m_position = pos;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
            }

        private:
            vec3 m_position;
            float m_radius;
        };
    }
}