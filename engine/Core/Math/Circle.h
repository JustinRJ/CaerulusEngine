#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Circle
        {
        public:
            Circle() = delete;
            ~Circle() = default;

            Circle(vec2 pos, float radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            bool IsPointInside(vec2 point) const
            {
                return length(m_pos - point) <= m_radius;
            }

            bool IsIntersecting(const Circle& circle) const
            {
                return length(m_pos - circle.Position()) <= m_radius + circle.Radius();
            }

            float Radius() const
            {
                return m_radius;
            }

            float& Radius()
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
            float m_radius;
            vec2 m_pos;
        };
    }
}
