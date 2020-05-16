#include "stdafx.h"

#include "DirectionalLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        DirectionalLight::DirectionalLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
            Light(shader, camera)
        {
        }

        const vec3& DirectionalLight::GetDirection() const
        {
            return m_Direction;
        }

        void DirectionalLight::SetDirection(const vec3& direction)
        {
            m_Direction = direction;
        }
    }
}