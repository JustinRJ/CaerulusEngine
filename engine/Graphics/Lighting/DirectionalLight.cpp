#include "stdafx.h"

#include "DirectionalLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        DirectionalLight::DirectionalLight(const glm::vec4& colour, const glm::vec3& direction) :
            Light(colour),
            m_Direction(direction)
        {
        }

        DirectionalLight::~DirectionalLight()
        {}

        const glm::vec3& DirectionalLight::GetDirection() const
        {
            return m_Direction;
        }

        void DirectionalLight::SetDirection(const glm::vec3& direction)
        {
            m_Direction = direction;
        }

        void DirectionalLight::RenderToShader(unsigned int ID, const Shaders::Shader& shader, const glm::mat4& view) const
        {
            glm::vec3 lightDirectionViewSpace = glm::vec3(view * glm::vec4(m_Direction, 0.0f));

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