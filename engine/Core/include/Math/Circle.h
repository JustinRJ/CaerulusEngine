#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Circle
        {
        public:
            Circle() = default;

            Circle(const vec2& pos, float radius) :
                m_radius(radius),
                m_pos(pos)
            {}

            bool IsPointInside(const vec2& point) const
            {
                return length2(m_pos - point) <= std::pow(m_radius, 2);
            }

            bool IsIntersecting(const Circle& circle) const
            {
                return length2(m_pos - circle.GetPosition()) <= std::pow(m_radius + circle.GetRadius(), 2);
            }

            float GetRadius() const
            {
                return m_radius;
            }

            void SetRadius(float radius)
            {
                m_radius = radius;
            }

            const vec2& GetPosition() const
            {
                return m_pos;
            }

            void SetPosition(const vec2& pos)
            {
                m_pos = pos;
            }

        private:
            float m_radius;
            vec2 m_pos;
        };
    }
}
