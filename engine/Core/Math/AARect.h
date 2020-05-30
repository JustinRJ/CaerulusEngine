#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE AARect
        {
        public:
            AARect() = delete;
            ~AARect() = default;

            AARect(const vec2& min, const vec2& max) :
                m_min(min),
                m_max(max)
            {}

            AARect(const AARect& rect) :
                m_min(rect.m_min),
                m_max(rect.m_max)
            {}

            AARect operator=(const AARect& rect)
            {
                m_min = rect.m_min;
                m_max = rect.m_max;
                return *this;
            }

            bool IsPointInside(vec2 point) const
            {
                if (point.x >= m_min.x && point.y >= m_min.y &&
                    point.x <= m_max.x && point.y <= m_max.y)
                {
                    return true;
                }
                return false;
            }

            bool IsIntersecting(const AARect& rect) const
            {
                return IsPointInside(rect.m_min) || IsPointInside(rect.m_max);
            }

            vec2& Min()
            {
                return m_min;
            }

            const vec2& Min() const
            {
                return m_min;
            }

            vec2& Max()
            {
                return m_max;
            }

            const vec2& Max() const
            {
                return m_max;
            }

        private:
            vec2 m_min;
            vec2 m_max;
        };
    }
}