#pragma once

#include <glew.h>
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace Geometry
    {
        class Geometry
        {
        protected:

            GLuint m_VAO = NULL;
            GLuint m_VBO = NULL;
            glm::mat4& m_Transform;

            Geometry(const glm::mat4& transform);
            virtual ~Geometry();

        public:
            virtual void Draw() const = 0;
            GLuint GetVAO() const;
            GLuint GetVBO() const;

            void SetTransform(glm::mat4& transform);
            glm::mat4& GetTransform() const;
        };
    }
}
