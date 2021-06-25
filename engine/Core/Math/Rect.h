#pragma once

#include <algorithm>

#include "Math.h"
#include "AARect.h"

namespace Core
{
    namespace Math
    {
        class Rect
        {
        public:
            Rect() = default;
            ~Rect() = default;

            Rect(const mat3& transform) :
                m_T(transform),
                m_aa(TransformVector(m_T, s_min),
                     TransformVector(m_T, s_max))
            {}

            const mat3& GetTransform() const
            {
                return m_T;
            }

            void SetTransform(const mat3& m)
            {
                m_T = m;
                m_aa.SetMin(TransformVector(m_T, s_min));
                m_aa.SetMax(TransformVector(m_T, s_max));
            }

            const AARect& GetAA() const
            {
                return m_aa;
            }

            bool IsIntersecting(const Rect& rect) const
            {
                if (!m_aa.IsIntersecting(rect.GetAA()))
                {
                    return false;
                }

                std::vector<vec2> corners = GetCorners();
                for (unsigned int i = 0u; i < corners.size(); ++i)
                {
                    vec2 current = corners[i];
                    vec2 next = corners[i == corners.size() - 1 ? 0 : i + 1];
                    vec2 edge = next - current;
                    vec2 axis = { -edge[1], edge[0] };

                    float aMinProj = std::numeric_limits<float>::max();
                    float bMinProj = std::numeric_limits<float>::max();
                    float aMaxProj = std::numeric_limits<float>::min();
                    float bMaxProj = std::numeric_limits<float>::min();

                    for (const vec2& v : corners)
                    {
                        float proj = dot(axis, v);
                        aMinProj = std::min(aMinProj, proj);
                        aMaxProj = std::max(aMaxProj, proj);
                    }

                    for (const vec2& v : rect.GetCorners())
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

            vec2 GetCenter() const
            {
                vec2 min = TransformVector(m_T, m_aa.GetMin());
                vec2 max = TransformVector(m_T, m_aa.GetMax());
                return vec2((min.x + max.x) / 2.f, (min.y + max.y) / 2.f);
            }

            std::vector<vec2> GetCorners() const
            {
                std::vector<vec2> corners(m_aa.GetCorners().size());
                std::transform(m_aa.GetCorners().begin(), m_aa.GetCorners().end(), corners.begin(), [&](const vec2& v)
                {
                    return TransformVector(m_T, v);
                });
                return corners;
            }

        private:
            mat3 m_T;
            AARect m_aa;
            static const vec2 s_min;
            static const vec2 s_max;
        };
        const vec2 Rect::s_min = { -0.5f, -0.5f };
        const vec2 Rect::s_max = { 0.5f, 0.5f };
    }
}
