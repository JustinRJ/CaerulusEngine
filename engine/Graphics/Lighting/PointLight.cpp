#include "stdafx.h"

#include "PointLight.h"
#include "Graphics/PipeLine/Shader.h"
#include "Core/Math/Camera.h"

namespace
{
    using namespace Core::Math;
    using namespace Graphics::PipeLine;
}

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
            return m_position;
        }

        float PointLight::GetRadius() const
        {
            return m_radius;
        }

        void PointLight::SetPosition(const vec3& pos)
        {
            m_position = pos;
        }

        void PointLight::SetRadius(float radius)
        {
            m_radius = radius;
        }
    }
}