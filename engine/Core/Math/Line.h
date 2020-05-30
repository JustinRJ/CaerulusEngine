#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class CAERULUS_CORE Line
        {
        public:
            Line() = delete;
            ~Line() = default;

            Line(vec3 start, vec3 end) :
                m_start(start),
                m_end(end),
                m_direction(normalize(end - start)),
                m_length(m_direction.length())
            {}

            Line(const Line& line) :
                m_start(line.m_start),
                m_end(line.m_end),
                m_direction(line.m_direction),
                m_length(m_direction.length())
            {}

            const vec3& Start() const
            {
                return m_start;
            }

            const vec3& End() const
            {
                return m_end;
            }

            const vec3& Direction() const
            {
                return m_direction;
            }

            double Length() const
            {
                return m_length;
            }

            vec3 GetOrthogonalPoint(const vec3& point) const
            {
                const vec3 startToPoint = m_start - point;
                return startToPoint - dot(startToPoint, m_direction) * m_direction;
            }

            bool IsPointOrthogonal(const vec3& point) const
            {
                const vec3 startToPoint = m_start - point;
                const double pointProjectedOntoLine = dot(startToPoint, m_direction);

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
            const vec3 m_start;
            const vec3 m_end;
            const vec3 m_direction;
            const double m_length;
        };
    }
}
