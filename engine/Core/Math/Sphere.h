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

            Sphere(const Sphere& sphere) :
                m_radius(sphere.m_radius),
                m_pos(sphere.m_pos)
            {}

            Sphere(Sphere&& sphere) :
                m_radius(std::move(sphere.m_radius)),
                m_pos(std::move(sphere.m_pos))
            {}

            Sphere& operator=(const Sphere& sphere)
            {
                m_radius = sphere.m_radius;
                m_pos = sphere.m_pos;
                return *this;
            }

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
