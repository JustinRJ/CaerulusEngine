#include "stdafx.h"

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        Light::Light(const glm::vec4& colour) :
            m_Colour(colour)
        {
        }

        Light::~Light()
        {
        }

        const glm::vec4& Light::GetColour() const
        {
            return m_Colour;
        }

        void Light::SetColour(const glm::vec4& colour)
        {
            m_Colour = colour;
        }
    }
}