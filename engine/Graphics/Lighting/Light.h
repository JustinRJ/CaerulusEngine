#pragma once

#include <glm.hpp>
#include <glew.h>
#include "../shader/Shader.h"

namespace Graphics
{
    namespace Light
    {
        class Light
        {
        public:

            Light(const glm::vec4& colour);
            virtual ~Light();
            virtual void SetColour(const glm::vec4& colour);
            virtual const glm::vec4& GetColour() const;

            virtual void RenderToShader(unsigned int ID, const Shaders::Shader& shader, const glm::mat4& view) const = 0;

        protected:
            glm::vec4 m_Colour;
        };
    }
}
