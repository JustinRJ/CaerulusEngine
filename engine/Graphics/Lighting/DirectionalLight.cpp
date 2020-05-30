#include "stdafx.h"

#include "DirectionalLight.h"
#include <string>
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
        DirectionalLight::DirectionalLight(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
            Light(shader, camera)
        {
        }

        const vec3& DirectionalLight::GetDirection() const
        {
            return m_direction;
        }

        void DirectionalLight::SetDirection(const vec3& direction)
        {
            m_direction = direction;
        }
    }
}