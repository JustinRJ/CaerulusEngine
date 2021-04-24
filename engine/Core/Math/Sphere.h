#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Sphere
        {
        public:
            Sphere() = delete;
            ~Sphere() = default;

            Sphere(vec3 pos, float radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            bool IsPointInside(vec3 point) const
            {
                return length(m_pos - point) <= m_radius;
            }

            bool IsIntersecting(const Sphere& sphere) const
            {
                return length(m_pos - sphere.Position()) <= m_radius + sphere.Radius();
            }

            bool IsIntersecting(const Circle& circle) const
            {
                return length(m_pos - vec3(circle.Position(), m_pos.z)) <= m_radius + circle.Radius();
            }

            float Radius() const
            {
                return m_radius;
            }

            float& Radius()
            {
                return m_radius;
            }

            vec3 Position() const
            {
                return m_pos;
            }

            vec3& Position()
            {
                return m_pos;
            }

        private:
            float m_radius;
            vec3 m_pos;
        };
    }
}
