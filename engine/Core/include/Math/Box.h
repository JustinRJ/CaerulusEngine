#pragma once

#include <algorithm>

#include "AABox.h"
#include "Transform.h"

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
                m_aa(TransformVector(m_T.GetMatrix(), s_min),
                     TransformVector(m_T.GetMatrix(), s_max))
            {}

            const mat4& GetTransform() const
            {
                return m_T.GetMatrix();
            }

            void SetTransform(const mat4& m)
            {
                m_T = m;
                m_aa.SetMin(TransformVector(m_T.GetMatrix(), s_min));
                m_aa.SetMax(TransformVector(m_T.GetMatrix(), s_max));
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

                std::vector<vec3> corners = GetCorners();
                for (unsigned int i = 0u; i < corners.size(); ++i)
                {
                    vec3 current = corners[i];
                    vec3 next = corners[i == corners.size() - 1 ? 0 : i + 1];
                    vec3 last = corners[i == 0 ? corners.size() - 1 : i - 1];
                    vec3 edge1 = next - current;
                    vec3 edge2 = last - current;
                    vec3 axis = normalize(cross(edge1, edge2));

                    float aMinProj = std::numeric_limits<float>::max();
                    float bMinProj = std::numeric_limits<float>::max();
                    float aMaxProj = std::numeric_limits<float>::min();
                    float bMaxProj = std::numeric_limits<float>::min();

                    for (const vec3& v : corners)
                    {
                        float proj = dot(axis, v);
                        aMinProj = std::min(aMinProj, proj);
                        aMaxProj = std::max(aMaxProj, proj);
                    }

                    for (const vec3& v : cube.GetCorners())
                    {
                        float proj = dot(axis, v);
                        bMinProj = std::min(bMinProj, proj);
                        bMaxProj = std::max(bMaxProj, proj);
                    }

                    if (aMaxProj < bMinProj || aMinProj > bMaxProj)
                    {
                        return true;
                    }
                }
                return false;
            }

            vec3 GetCenter() const
            {
                vec3 min = TransformVector(m_T.GetMatrix(), m_aa.GetMin());
                vec3 max = TransformVector(m_T.GetMatrix(), m_aa.GetMax());
                return vec3((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f);
            }

            std::vector<vec3> GetCorners() const
            {
                std::vector<vec3> corners(m_aa.GetCorners().size());
                std::transform(m_aa.GetCorners().begin(), m_aa.GetCorners().end(), corners.begin(), [&](const vec3& v)
                {
                    return TransformVector(m_T.GetMatrix(), v);
                });
                return corners;
            }

        private:
            Transform m_T;
            AABox m_aa;
            static const vec3 s_min;
            static const vec3 s_max;
        };
        const vec3 Box::s_min = { -0.5f, -0.5f, -0.5f };
        const vec3 Box::s_max = { 0.5f, 0.5f, 0.5f };
    }
}