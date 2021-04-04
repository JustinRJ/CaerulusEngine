#pragma once

#include "AACube.h"
#include "Transform.h"

namespace Core
{
    namespace Math
    {
        class Cube
        {
        public:
            Cube() = delete;
            ~Cube() = default;

            Cube(const mat4& transform) :
                m_T(transform),
                m_aa(TransformVector(m_T.GetMatrix(), s_min),
                     TransformVector(m_T.GetMatrix(), s_max))
            {}

            Cube(const Cube& cube) :
                m_T(cube.m_T),
                m_aa(cube.m_aa)
            {}

            Cube& operator=(const Cube& cube)
            {
                m_T = cube.m_T;
                return *this;
            }

            const mat4& GetTransform() const
            {
                return m_T.GetMatrix();
            }

            void SetTransform(const mat4& m)
            {
                m_T = m;
                m_aa.Min() = TransformVector(m_T.GetMatrix(), s_min);
                m_aa.Max() = TransformVector(m_T.GetMatrix(), s_max);
            }

            const AACube& GetAA() const
            {
                return m_aa;
            }

            bool IsIntersecting(const Cube& cube) const
            {
                if (m_aa.IsIntersecting(cube.GetAA()))
                {
                    // TODO
                    std::vector<vec3> corners = GetCorners();
                    // SAT
                    return false;
                }
                return false;
            }

            vec3 GetCenter() const
            {
                vec3 min = TransformVector(m_T.GetMatrix(), m_aa.Min());
                vec3 max = TransformVector(m_T.GetMatrix(), m_aa.Max());
                return vec3((min.x + max.x) / 2.f, (min.y + max.y) / 2.f, (min.z + max.z) / 2.f);
            }

            std::vector<vec3> GetCorners() const
            {
                std::vector<vec3> corners;
                std::transform(m_aa.GetCorners().begin(), m_aa.GetCorners().end(), corners.begin(), [&](const vec3& v)
                {
                    return TransformVector(m_T.GetMatrix(), v);
                });
                return corners;
            }

            std::vector<vec3> GetNormals() const
            {
                std::vector<vec3> corners = GetCorners();
                std::vector<vec3> normals {
                normalize(cross(corners[0] + corners[1], corners[0] + corners[3])),
                normalize(cross(corners[0] + corners[4], corners[0] + corners[1])),
                normalize(cross(corners[5] + corners[1], corners[5] + corners[6])),
                normalize(cross(corners[3] + corners[2], corners[3] + corners[7])),
                normalize(cross(corners[0] + corners[3], corners[0] + corners[4])),
                normalize(cross(corners[7] + corners[6], corners[7] + corners[4])) };
                return normals;
            }

        private:
            static const vec3 s_min;
            static const vec3 s_max;
            Transform m_T;
            AACube m_aa;
        };
        const vec3 Cube::s_min = { -0.5f, -0.5f, -0.5f };
        const vec3 Cube::s_max = { 0.5f, 0.5f, 0.5f };
    }
}