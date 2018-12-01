#include "stdafx.h"

#include "PointLight.h"
#include <string>

namespace Graphics
{
    namespace Light
    {
        PointLight::PointLight(const glm::vec4& colour, const glm::vec3& position, float radius) :
            Light(colour) ,
            m_Position(position),
            m_Radius(radius)
        {
        }

        PointLight::~PointLight()
        {
        }

        const glm::vec3& PointLight::GetPosition() const
        {
            return m_Position;
        }

        float PointLight::GetRadius() const
        {
            return m_Radius;
        }

        void PointLight::SetPosition(const glm::vec3& pos)
        {
            m_Position = pos;
        }

        void PointLight::SetRadius(float radius)
        {
            m_Radius = radius;
        }

        void PointLight::RenderToShader(unsigned int ID, const Shaders::Shader& shader, const glm::mat4& view) const
        {
            glm::vec3 lightPositionViewSpace = glm::vec3(view * glm::vec4(m_Position, 1.0f));

            glUniform3f(glGetUniformLocation(shader.GetHandle(), ("lightPointArray[" + std::to_string(ID) + "].position").c_str()),
                lightPositionViewSpace.x,
                lightPositionViewSpace.y, 
                lightPositionViewSpace.z);

            glUniform4f(glGetUniformLocation(shader.GetHandle(), ("lightPointArray[" + std::to_string(ID) + "].color").c_str()),
                m_Colour.r, 
                m_Colour.g, 
                m_Colour.b, 
                m_Colour.a);

            glUniform1f(glGetUniformLocation(shader.GetHandle(), ("lightPointArray[" + std::to_string(ID) + "].radius").c_str()),
                m_Radius);
        }
    }
}