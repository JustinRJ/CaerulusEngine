#include "stdafx.h"

#include "Light.h"

namespace Graphics
{
    namespace Light
    {
        Light::Light(const vec4& colour) :
            m_Colour(colour)
        {
        }

        const vec4& Light::GetColour() const
        {
            return m_Colour;
        }

        void Light::SetColour(const vec4& colour)
        {
            m_Colour = colour;
        }
    }
}