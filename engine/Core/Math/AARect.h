#pragma once

#include <vector>

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class AARect
        {
        public:
            AARect() = default;
            ~AARect() = default;

            AARect(const vec2& min, const vec2& max) :
                m_min(min),
                m_max(max)
            {}

            bool IsPointInside(const vec2& point) const
            {
                return 
                    point.x >= m_min.x && point.y >= m_min.y &&
                    point.x <= m_max.x && point.y <= m_max.y;
            }

            bool IsIntersecting(const AARect& rect) const
            {
                return IsPointInside(rect.m_min) || IsPointInside(rect.m_max);
            }

            void SetMin(const vec2& min)
            {
                m_min = min;
            }

            const vec2& GetMin() const
            {
                return m_min;
            }

            void SetMax(const vec2& max)
            {
                m_max = max;
            }

            const vec2& GetMax() const
            {
                return m_max;
            }

            vec2 GetCenter() const
            {
                return vec2((m_min.x + m_max.x) / 2.f, (m_min.y + m_max.y) / 2.f);
            }

            std::vector<vec2> GetCorners() const
            {
                std::vector<vec2> corners {
                m_min,
                vec2(m_min.x, m_max.y),
                m_max,
                vec2(m_max.x, m_min.y) };
                return corners;
            }

        private:
            vec2 m_min;
            vec2 m_max;
        };
    }
}