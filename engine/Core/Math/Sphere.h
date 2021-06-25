#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Sphere
        {
        public:
            Sphere() = default;
            ~Sphere() = default;

            Sphere(const vec3& pos, float radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            bool IsPointInside(const vec3& point) const
            {
                return length2(m_pos - point) <= std::pow(m_radius, 2);
            }

            bool IsIntersecting(const Sphere& sphere) const
            {
                return length2(m_pos - sphere.GetPosition()) <= std::pow(m_radius + sphere.GetRadius(), 2);
            }

            float GetRadius() const
            {
                return m_radius;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
            }

            const vec3& GetPosition() const
            {
                return m_pos;
            }

            void SetPosition(const vec3& pos)
            {
                m_pos = pos;
            }

        private:
            float m_radius;
            vec3 m_pos;
        };
    }
}
