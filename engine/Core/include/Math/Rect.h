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
                return IsIntersecting2D(GetCorners(), rect.GetCorners());
            }

            vec2 GetCenter() const
            {
                vec2 min = TransformVector(m_T, m_aa.GetMin());
                vec2 max = TransformVector(m_T, m_aa.GetMax());
                return vec2((min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f);
            }

            std::vector<vec2> GetCorners() const
            {
                std::vector<vec2> corners(m_aa.GetCorners().size());
                std::transform(std::begin(GetCorners()), std::end(m_aa.GetCorners()), std::begin(corners), [&](const vec2& v)
                {
                    return TransformVector(m_T, v);
                });
                return corners;
            }

        private:
            mat3 m_T;
            AARect m_aa;
            inline static const vec2 s_min = { -0.5f, -0.5f };
            inline static const vec2 s_max = { 0.5f, 0.5f };
        };
    }
}
