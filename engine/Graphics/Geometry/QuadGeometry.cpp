#include "stdafx.h"

#include "QuadGeometry.h"

namespace Graphics
{
    namespace Geometry
    {
        const GLfloat QuadGeometry::s_QuadVertices[] =
        {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        QuadGeometry::QuadGeometry(const glm::mat4& transform) :
            Geometry(transform)
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(s_QuadVertices), s_QuadVertices, GL_STATIC_DRAW);
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
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);
        }
    }
}