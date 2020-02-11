#include "stdafx.h"

#include "PointLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        PointLight::PointLight(const vec4& colour, const vec3& position, float radius) :
            Light(colour) ,
            m_Position(position),
            m_Radius(radius)
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

        void PointLight::UpdateUniforms(const std::vector<std::string>& uniforms, const PipeLine::Shader& shader, const mat4& view) const
        {
            vec3 lightPositionViewSpace = vec3(view * vec4(m_Position, 1.0f));

            glUniform4f(glGetUniformLocation(shader.GetHandle(), uniforms.at(0).c_str()),
                m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a);

            glUniform3f(glGetUniformLocation(shader.GetHandle(), uniforms.at(1).c_str()),
                lightPositionViewSpace.x, lightPositionViewSpace.y, lightPositionViewSpace.z);

            glUniform1f(glGetUniformLocation(shader.GetHandle(), uniforms.at(2).c_str()), m_Radius);
        }
    }
}