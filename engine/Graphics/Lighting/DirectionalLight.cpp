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

        DirectionalLight::~DirectionalLight()
        {}

        const vec3& DirectionalLight::GetDirection() const
        {
            return m_Direction;
        }

        void DirectionalLight::SetDirection(const vec3& direction)
        {
            m_Direction = direction;
        }

        void DirectionalLight::RenderToShader(unsigned int ID, const PipeLine::Shader& shader, const mat4& view) const
        {
            vec3 lightDirectionViewSpace = vec3(view * vec4(m_Direction, 0.0f));

            glUniform3f(glGetUniformLocation(shader.GetHandle(),
                ("lightDirectionalArray[" + std::to_string(ID) + "].direction").c_str()),
                lightDirectionViewSpace.x,
                lightDirectionViewSpace.y,
                lightDirectionViewSpace.z);

            glUniform4f(glGetUniformLocation(shader.GetHandle(),
                ("lightDirectionalArray[" + std::to_string(ID) + "].color").c_str()),
                m_Colour.r,
                m_Colour.g,
                m_Colour.b,
                m_Colour.a);
        }

    }
}