#pragma once

#include <vector>
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

            AACube(const AACube& cube) :
                m_min(cube.m_min),
                m_max(cube.m_max)
            {}

            AACube(AACube&& cube) :
                m_min(std::move(cube.m_min)),
                m_max(std::move(cube.m_max))
            {}

            AACube& operator=(const AACube& cube)
            {
                m_min = cube.m_min;
                m_max = cube.m_max;
                return *this;
            }

            bool IsPointInside(vec3 point) const
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
                std::vector<vec3> corners(8);
                corners[0] = m_min;
                corners[1] = vec3(m_min.x, m_max.y, m_min.z);
                corners[2] = vec3(m_max.x, m_max.y, m_min.z);
                corners[3] = vec3(m_max.x, m_min.y, m_min.z);
                corners[4] = vec3(m_min.x, m_min.y, m_max.z);
                corners[5] = vec3(m_min.x, m_max.y, m_max.z);
                corners[6] = m_max;
                corners[7] = vec3(m_max.x, m_min.y, m_max.z);
                return corners;
            }

            static std::vector<vec3> GetNormals()
            {
                std::vector<vec3> normals(6);
                normals[0] = vec3(0.f, 0.f, -1.f);
                normals[1] = vec3(-1.f, 0.f, 0.f);
                normals[2] = vec3(0.f, 1.f, 0.f);
                normals[3] = vec3(-1.f, 0.f, 0.f);
                normals[4] = vec3(0.f, -1.f, 0.f);
                normals[5] = vec3(0.f, 0.f, 1.f);
                return normals;
            }

        private:
            vec3 m_min;
            vec3 m_max;
        };
    }
}