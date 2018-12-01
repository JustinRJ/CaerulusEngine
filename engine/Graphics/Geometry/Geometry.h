#pragma once

#include <GL/glew.h>
#include "../../Core/Math/MathHelper.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            class Geometry
            {
            protected:

                GLuint m_VAO = NULL;
                GLuint m_VBO = NULL;
                glm::mat4& m_Model;

                Geometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation);
                Geometry(glm::mat4& transform);
                virtual ~Geometry();

            public:
                virtual void Draw() const = 0;
                GLuint GetVAO() const;
                GLuint GetVBO() const;

                glm::vec3 GetPosition() const;
                glm::quat GetRotation() const;
                glm::vec3 GetScale() const;

                void SetPosition(const glm::vec3& position);
                void SetRotation(const glm::quat& rotationAxis);
                void Scale(const glm::vec3& scale);
            };
        }
    }
}
