#pragma once

#include <glew.h>
#include "../PipeLine/Shader.h"
#include "../../Core/Math/MathHelper.h"

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

            void SetColour(const vec4& colour);
            const vec4& GetColour() const;

            virtual void RenderToShader(unsigned int ID, const PipeLine::Shader& shader, const mat4& view) const = 0;

        protected:
            vec4 m_Colour;
        };
    }
}
