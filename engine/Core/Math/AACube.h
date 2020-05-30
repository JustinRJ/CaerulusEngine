#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE AACube
        {
        public:
            AACube() = delete;
            ~AACube() = default;

            AACube(const vec3& min, const vec3& max) :
                m_min(min),
                m_max(max)
            {}

            AACube(const AACube& cube) :
                m_min(cube.m_min),
                m_max(cube.m_max)
            {}

            AACube operator=(const AACube& cube)
            {
                m_min = cube.m_min;
                m_max = cube.m_max;
                return *this;
            }

            bool IsPointInside(vec3 point) const
            {
                if (point.x >= m_min.x && point.y >= m_min.y && point.z >= m_min.z &&
                    point.x <= m_max.x && point.y <= m_max.y && point.z <= m_max.z)
                {
                    return true;
                }
                return false;
            }

            bool IsIntersecting(const AACube& cube) const
            {
                return IsPointInside(cube.m_min) || IsPointInside(cube.m_max);
            }

            vec3& Min()
            {
                return m_min;
            }

            const vec3& Min() const
            {
                return m_min;
            }

            vec3& Max()
            {
                return m_max;
            }

            const vec3& Max() const
            {
                return m_max;
            }

        private:
            vec3 m_min;
            vec3 m_max;
        };
    }
}