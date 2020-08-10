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
                std::vector<vec2> corners(4);
                corners[0] = m_min;
                corners[1] = vec2(m_min.x, m_max.y);
                corners[2] = m_max;
                corners[3] = vec2(m_max.x, m_min.y);
                return corners;
            }

            static std::vector<vec2> GetNormals()
            {
                std::vector<vec2> normals(4);
                normals[0] = vec2(-1.f, 0.f);
                normals[1] = vec2(0.f, 1.f);
                normals[2] = vec2(1.f, 0.f);
                normals[3] = vec2(0.f, -1.f);
                return normals;
            }

        private:
            vec2 m_min;
            vec2 m_max;
        };
    }
}