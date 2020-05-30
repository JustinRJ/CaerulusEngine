#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE Circle
        {
        public:
            Circle() = delete;
            ~Circle() = default;

            Circle(vec2 pos, double radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            Circle(const Circle& sphere) :
                m_radius(sphere.m_radius),
                m_pos(sphere.m_pos)
            {}

            bool IsPointInside(vec2 point) const
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

            vec2 Position() const
            {
                return m_pos;
            }

            vec2& Position()
            {
                return m_pos;
            }

        private:
            double m_radius;
            vec2 m_pos;
        };
    }
}
