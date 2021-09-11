#pragma once

#include <vector>

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class AABox
        {
        public:
            AABox() = default;

            AABox(const vec3& min, const vec3& max) :
                m_min(min),
                m_max(max)
            {}

            bool IsPointInside(const vec3& point) const
            {
                return 
                    point.x >= m_min.x && point.y >= m_min.y && point.z >= m_min.z &&
                    point.x <= m_max.x && point.y <= m_max.y && point.z <= m_max.z;
            }

            bool IsIntersecting(const AABox& cube) const
            {
                return IsPointInside(cube.m_min) || IsPointInside(cube.m_max);
            }

            void SetMin(const vec3& min)
            {
                m_min = min;
            }

            const vec3& GetMin() const
            {
                return m_min;
            }

            void SetMax(const vec3& max)
            {
                 m_max = max;
            }

            const vec3& GetMax() const
            {
                return m_max;
            }

            vec3 GetCenter() const
            {
                return vec3((m_min.x + m_max.x) * 0.5f, (m_min.y + m_max.y) * 0.5f, (m_min.z + m_max.z) * 0.5f);
            }

            std::vector<vec3> GetCorners() const
            {
                std::vector<vec3> corners {
                m_min,
                vec3(m_min.x, m_max.y, m_min.z),
                vec3(m_max.x, m_max.y, m_min.z),
                vec3(m_max.x, m_min.y, m_min.z),
                vec3(m_min.x, m_min.y, m_max.z),
                vec3(m_min.x, m_max.y, m_max.z),
                m_max,
                vec3(m_max.x, m_min.y, m_max.z) };
                return corners;
            }

        private:
            vec3 m_min;
            vec3 m_max;
        };
    }
}