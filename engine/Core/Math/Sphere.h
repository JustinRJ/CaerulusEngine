#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE Sphere
        {
        public:
            Sphere() = delete;
            ~Sphere() = default;

            Sphere(vec3 pos, double radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            Sphere(const Sphere& sphere) :
                m_radius(sphere.m_radius),
                m_pos(sphere.m_pos)
            {}

            bool IsPointInside(vec3 point) const
            {
                return (m_pos - point).length() <= m_radius;
            }

            double Radius() const
            {
                return m_radius;
            }

            double& Radius()
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
            double m_radius;
            vec3 m_pos;
        };
    }
}
