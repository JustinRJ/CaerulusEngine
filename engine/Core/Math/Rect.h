#pragma once

#include "AARect.h"

namespace Core
{
    namespace Math
    {
        class Rect
        {
        public:
            Rect() = delete;
            ~Rect() = default;

            Rect(const mat3& transform) :
                m_T(transform),
                m_aa(TransformVector(m_T, s_min),
                     TransformVector(m_T, s_max))
            {}

            Rect(const Rect& rect) :
                m_T(rect.m_T),
                m_aa(rect.m_aa)
            {}

            Rect& operator=(const Rect& rect)
            {
                m_T = rect.m_T;
                m_aa = rect.m_aa;
                return *this;
            }

            const mat3& GetTransform() const
            {
                return m_T;
            }

            void SetTransform(const mat3& m)
            {
                m_T = m;
                m_aa.Min() = TransformVector(m_T, s_min);
                m_aa.Max() = TransformVector(m_T, s_max);
            }

            const AARect& GetAA() const
            {
                return m_aa;
            }

            bool IsIntersecting(const Rect& rect) const
            {
                if (m_aa.IsIntersecting(rect.GetAA()))
                {
                    // TODO
                    vec2 center = m_aa.GetCenter();
                    std::vector<vec2> corners = GetCorners();
                    std::vector<vec2> normals = GetNormals();
                    // SAT
                    return false;
                }
                return false;
            }

            vec2 GetCenter() const
            {
                vec2 min = TransformVector(m_T, m_aa.Min());
                vec2 max = TransformVector(m_T, m_aa.Max());
                return vec2((min.x + max.x) / 2.f, (min.y + max.y) / 2.f);
            }

            std::vector<vec2> GetCorners() const
            {
                std::vector<vec2> corners;
                std::transform(m_aa.GetCorners().begin(), m_aa.GetCorners().end(), corners.begin(), [&](const vec2& v)
                {
                    return TransformVector(m_T, v);
                });
                return corners;
            }

            std::vector<vec2> GetNormals() const
            {
                std::vector<vec2> normals(4);
                std::vector<vec2> corners = GetCorners();
                vec2 v0 = normalize(corners[0] - corners[1]);
                vec2 v1 = normalize(corners[1] + corners[2]);
                vec2 v2 = normalize(corners[2] - corners[3]);
                vec2 v3 = normalize(corners[3] + corners[0]);

                normals[0] = vec2(v0.y, v0.x);
                normals[1] = vec2(v1.y, v1.x);
                normals[2] = vec2(v2.y, v2.x);
                normals[3] = vec2(v3.y, v3.x);

                return normals;
            }

        private:
            static const vec2 s_min;
            static const vec2 s_max;
            mat3 m_T;
            AARect m_aa;
        };
        const vec2 Rect::s_min = { -0.5f, -0.5f };
        const vec2 Rect::s_max = { 0.5f, 0.5f };
    }
}
