#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Ray
        {
        public:
            Ray() = default;

            Ray(const vec3& origin, const vec3& direction) :
                m_origin(origin),
                m_direction(normalize(direction))
            {}

            const vec3& GetOrigin() const
            {
                return m_origin;
            }

            const vec3& GetDirection() const
            {
                return m_direction;
            }

            void SetOrigin(const vec3& origin)
            {
                m_origin = origin;
            }

            void SetDirection(const vec3& direction)
            {
                m_direction = direction;
            }

        private:
            vec3 m_origin;
            vec3 m_direction;
        };
    }
}
