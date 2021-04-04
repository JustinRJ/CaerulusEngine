#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class AARect
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

            AARect& operator=(const AARect& rect)
            {
                m_min = rect.m_min;
                m_max = rect.m_max;
                return *this;
            }

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

            static std::vector<vec2> GetNormals()
            {
                std::vector<vec2> normals {
                vec2(-1.f, 0.f),
                vec2(0.f, 1.f),
                vec2(1.f, 0.f),
                vec2(0.f, -1.f) };
                return normals;
            }

        private:
            vec2 m_min;
            vec2 m_max;
        };
    }
}