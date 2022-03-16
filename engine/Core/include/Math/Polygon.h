#pragma once

#include "Circle.h"
#include "Transform.h"

namespace Core
{
    namespace Math
    {
        class Polygon
        {
        public:
            Polygon() = default;

            Polygon(const std::vector<vec2>& points) :
                m_points(points)
            {
                UpdateCircle();
            }

            bool IsIntersecting(const Polygon& polygon) const
            {
                if (!m_circle.IsIntersecting(polygon.m_circle))
                {
                    return false;
                }
                return IsIntersecting2D(m_points, polygon.GetPoints());
            }

            const std::vector<vec2>& GetPoints() const
            {
                return m_points;
            }

            void SetPoints(const std::vector<vec2>& points)
            {
                m_points = points;
                UpdateCircle();
            }

        private:

            void UpdateCircle()
            {
                vec2 centerAverage;
                for (vec2 point : m_points)
                {
                    centerAverage += point;
                }
                centerAverage = vec2(centerAverage.x / m_points.size(), centerAverage.y / m_points.size());

                float maxDistanceSquared = std::numeric_limits<float>().min();
                for (vec2 point : m_points)
                {
                    maxDistanceSquared = std::max(maxDistanceSquared, length2(centerAverage - point));
                }

                m_circle.SetPosition(centerAverage);
                m_circle.SetRadius(sqrt(maxDistanceSquared));
            }

            Circle m_circle;
            std::vector<vec2> m_points;
        };
    }
}
