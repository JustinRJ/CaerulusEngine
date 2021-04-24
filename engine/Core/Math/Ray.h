#pragma once

#include "Math.h"

namespace Core
{
    namespace Math
    {
        class Ray
        {
        public:
            Ray() = delete;
            ~Ray() = default;

            Ray(vec3 origin, vec3 direction) :
                m_origin(origin),
                m_direction(normalize(direction))
            {}

            const vec3& Origin() const
            {
                return m_origin;
            }

            const vec3& Direction() const
            {
                return m_direction;
            }

        private:
            const vec3 m_origin;
            const vec3 m_direction;
        };
    }
}
