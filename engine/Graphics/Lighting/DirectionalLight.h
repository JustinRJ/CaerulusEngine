#pragma once

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        class DirectionalLight : public Light
        {
        public:
            DirectionalLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
                Light(shader, camera)
            {}

            virtual ~DirectionalLight() = default;

            const vec3& GetDirection() const
            {
                return m_direction;
            }

            void SetDirection(const vec3& direction)
            {
                m_direction = direction;
            }

        protected:
            vec3 m_direction;
        };
    }
}