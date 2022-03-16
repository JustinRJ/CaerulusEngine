#pragma once

#include <algorithm>

#include "Math.h"
#include "AABox.h"

namespace Core
{
    namespace Math
    {
        class Box
        {
        public:
            Box() = default;

            Box(const mat4& transform) :
                m_T(transform),
                m_aa(TransformVector(m_T, s_min),
                     TransformVector(m_T, s_max))
            {}

            const mat4& GetTransform() const
            {
                return m_T;
            }

            void SetTransform(const mat4& m)
            {
                m_T = m;
                m_aa.SetMin(TransformVector(m_T, s_min));
                m_aa.SetMax(TransformVector(m_T, s_max));
            }

            const AABox& GetAA() const
            {
                return m_aa;
            }

            bool IsIntersecting(const Box& cube) const
            {
                if (!m_aa.IsIntersecting(cube.GetAA()))
                {
                    return false;
                }
                return IsIntersecting3D(GetCorners(), cube.GetCorners());
            }

            vec3 GetCenter() const
            {
                vec3 min = TransformVector(m_T, m_aa.GetMin());
                vec3 max = TransformVector(m_T, m_aa.GetMax());
                return vec3((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
            }

            std::vector<vec3> GetCorners() const
            {
                std::vector<vec3> corners(m_aa.GetCorners().size());
                std::transform(std::begin(m_aa.GetCorners()), std::end(m_aa.GetCorners()), std::begin(corners), [&](const vec3& v)
                {
                    return TransformVector(m_T, v);
                });
                return corners;
            }

        private:
            mat4 m_T;
            AABox m_aa;
            static const vec3 s_min;
            static const vec3 s_max;
        };
        const vec3 Box::s_min = { -0.5f, -0.5f, -0.5f };
        const vec3 Box::s_max = { 0.5f, 0.5f, 0.5f };
    }
}