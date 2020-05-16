#include "stdafx.h"

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        Light::Light(std::shared_ptr<Shader> shader, std::shared_ptr<Camera> camera) :
            m_Shader(shader),
            m_Camera(camera)
        {
        }

        const vec4& Light::GetColour() const
        {
            return m_Colour;
        }

        std::shared_ptr<Shader> Light::GetShader() const
        {
            return m_Shader;
        }

        std::shared_ptr<Camera> Light::GetCamera() const
        {
            return m_Camera;
        }

        void Light::SetColour(const vec4& colour)
        {
            m_Colour = colour;
        }
    }
}