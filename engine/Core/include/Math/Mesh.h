#pragma once

#include "Sphere.h"
#include "Transform.h"

namespace Core
{
    namespace Math
    {
        class Mesh
        {
        public:
            Mesh() = default;

            Mesh(const std::vector<vec3>& points) :
                m_points(points)
            {
                UpdateSphere();
            }

            bool IsIntersecting(const Mesh& mesh) const
            {
                if (!m_sphere.IsIntersecting(mesh.m_sphere))
                {
                    return false;
                }
                return IsIntersecting3D(m_points, mesh.GetPoints());
            }

            const std::vector<vec3>& GetPoints() const
            {
                return m_points;
            }

            void SetPoints(const std::vector<vec3>& points)
            {
                m_points = points;
                UpdateSphere();
            }

        private:

            void UpdateSphere()
            {
                vec3 centerAverage;
                for (vec3 point : m_points)
                {
                    centerAverage += point;
                }
                centerAverage = vec3(centerAverage.x / m_points.size(), centerAverage.y / m_points.size(), centerAverage.z / m_points.size());

                float maxDistanceSquared = std::numeric_limits<float>().min();
                for (vec3 point : m_points)
                {
                    maxDistanceSquared = std::max(maxDistanceSquared, length2(centerAverage - point));
                }

                m_sphere.SetPosition(centerAverage);
                m_sphere.SetRadius(sqrt(maxDistanceSquared));
            }

            Sphere m_sphere;
            std::vector<vec3> m_points;
        };
    }
}
