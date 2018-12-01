#include "stdafx.h"

#include "QuadGeometry.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            const GLfloat QuadGeometry::sm_QuadVertices[] =
            {
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };

            QuadGeometry::QuadGeometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation) : 
                Geometry(scale, rot, translation)
            {
                glBufferData(GL_ARRAY_BUFFER, sizeof(sm_QuadVertices), sm_QuadVertices, GL_STATIC_DRAW);
                glBindVertexArray(m_VAO);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

                glBindVertexArray(0);
            }

            QuadGeometry::~QuadGeometry()
            {
            }

            void QuadGeometry::Draw() const
            {
                glBindVertexArray(m_VAO);
                // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindVertexArray(0);
            }
        }
    }
}