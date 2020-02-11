#include "stdafx.h"

#include "DirectionalLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        DirectionalLight::DirectionalLight(const vec4& colour, const vec3& direction) :
            Light(colour),
            m_Direction(direction)
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

        void DirectionalLight::UpdateUniforms(const std::vector<std::string>& uniforms, const PipeLine::Shader& shader, const mat4& view) const
        {
            vec3 lightDirectionViewSpace = vec3(view * vec4(m_Direction, 0.0f));

            glUniform4f(glGetUniformLocation(shader.GetHandle(), uniforms.at(0).c_str()),
                m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a);

            glUniform3f(glGetUniformLocation(shader.GetHandle(), uniforms.at(1).c_str()),
                lightDirectionViewSpace.x, lightDirectionViewSpace.y, lightDirectionViewSpace.z);
        }
    }
}