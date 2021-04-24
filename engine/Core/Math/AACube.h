#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class AACube
        {
        public:
            AACube() = delete;
            ~AACube() = default;

            AACube(const vec3& min, const vec3& max) :
                m_min(min),
                m_max(max)
            {}

            bool IsPointInside(const vec3& point) const
            {
                return 
                    point.x >= m_min.x && point.y >= m_min.y && point.z >= m_min.z &&
                    point.x <= m_max.x && point.y <= m_max.y && point.z <= m_max.z;
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

            static std::vector<vec3> GetNormals()
            {
                std::vector<vec3> normals {
                vec3(0.f, 0.f, -1.f),
                vec3(-1.f, 0.f, 0.f),
                vec3(0.f, 1.f, 0.f),
                vec3(-1.f, 0.f, 0.f),
                vec3(0.f, -1.f, 0.f),
                vec3(0.f, 0.f, 1.f) };
                return normals;
            }

        private:
            vec3 m_min;
            vec3 m_max;
        };
    }
}