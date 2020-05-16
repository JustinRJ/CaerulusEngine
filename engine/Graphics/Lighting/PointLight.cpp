#include "stdafx.h"

#include "PointLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        PointLight::PointLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
            Light(shader, camera)
        {
        }

        const vec3& PointLight::GetPosition() const
        {
            return m_Position;
        }

        float PointLight::GetRadius() const
        {
            return m_Radius;
        }

        void PointLight::SetPosition(const vec3& pos)
        {
            m_Position = pos;
        }

        void PointLight::SetRadius(float radius)
        {
            m_Radius = radius;
        }
    }
}