#pragma once

#include <vector>

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

                std::vector<vec2> corners = GetPoints();
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

                    for (const vec2& v : polygon.GetPoints())
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
                vec2 centerAverage(0, 0);
                for (vec2 point : m_points)
                {
                    centerAverage.x += point.x;
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
