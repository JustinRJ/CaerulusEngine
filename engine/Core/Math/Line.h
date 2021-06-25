#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Line
        {
        public:
            Line() = default;
            ~Line() = default;

            Line(const vec3& start, const vec3& end) :
                m_start(start),
                m_end(end),
                m_direction(normalize(end - start)),
                m_length(length(end - start))
            {}

            const vec3& GetStart() const
            {
                return m_start;
            }

            const vec3& GetEnd() const
            {
                return m_end;
            }

            const vec3& GetDirection() const
            {
                return m_direction;
            }

            float GetLength() const
            {
                return m_length;
            }

            void SetStart(const vec3& start)
            {
                m_start = start;
                m_direction = normalize(m_end - m_start);
                m_length = length(m_end - m_start);
            }

            void SetEnd(const vec3& end)
            {
                m_end = end;
                m_direction = normalize(m_end - m_start);
                m_length = length(m_end - m_start);
            }

            vec3 GetOrthogonalPoint(const vec3& point) const
            {
                const vec3 startToPoint = m_start - point;
                return startToPoint - dot(startToPoint, m_direction) * m_direction;
            }

            bool IsPointOrthogonal(const vec3& point) const
            {
                const vec3 startToPoint = m_start - point;
                const float pointProjectedOntoLine = dot(startToPoint, m_direction);

                if (pointProjectedOntoLine >= 0 && pointProjectedOntoLine <= m_length)
                {
                    return true;
                }
                return false;
            }

            vec3 GetShortestVectorToPoint(const vec3& point) const
            {
                if (IsPointOrthogonal(point))
                {
                    return GetOrthogonalPoint(point);
                }

                const vec3 startToPoint = m_start - point;
                const vec3 endToPoint = m_end - point;

                if (length2(startToPoint) < length2(endToPoint))
                {
                    return startToPoint;
                }

                return endToPoint;
            }

        private:
            vec3 m_start;
            vec3 m_end;
            vec3 m_direction;
            float m_length;
        };
    }
}
