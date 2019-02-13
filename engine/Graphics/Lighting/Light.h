#pragma once

#include <glew.h>
#include "../shader/Shader.h"
#include "../../Core/Math/MathFacade.h"

namespace Graphics
{
    namespace Light
    {
        using namespace Core::Math;

        class Light
        {
        public:

            Light(const vec4& colour);
            virtual ~Light();
            virtual void SetColour(const vec4& colour);
            virtual const vec4& GetColour() const;

            virtual void RenderToShader(unsigned int ID, const Shaders::Shader& shader, const mat4& view) const = 0;

        protected:
            vec4 m_Colour;
        };
    }
}
