#include "stdafx.h"

#include "Light.h"
#include "Graphics/PipeLine/Shader.h"
#include "Graphics/Resource/Camera.h"

namespace
{
    using namespace Graphics::PipeLine;
    using namespace Graphics::Resource;
}

namespace Graphics
{
    namespace Light
    {
        Light::Light(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
            m_shader(shader),
            m_camera(camera)
        {
        }

        const vec4& Light::GetColour() const
        {
            return m_colour;
        }

        std::shared_ptr<Shader> Light::GetShader() const
        {
            return m_shader;
        }

        std::shared_ptr<Camera> Light::GetCamera() const
        {
            return m_camera;
        }

        void Light::SetColour(const vec4& colour)
        {
            m_colour = colour;
        }
    }
}