#include "stdafx.h"

#include "PlaneGeometry.h"

namespace Graphics
{
    namespace Geometry
    {
        const GLfloat PlaneGeometry::s_PlaneVertices[] =
        {
            0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
        };

        PlaneGeometry::PlaneGeometry() :
            Geometry(mat4())
        {
            GenerateBuffer();
        }

        PlaneGeometry::PlaneGeometry(const mat4& transform) :
            Geometry(transform)
        {
            GenerateBuffer();
        }

        PlaneGeometry::~PlaneGeometry()
        {
        }

        void PlaneGeometry::Draw() const
        {
            glBindVertexArray(m_VAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
            glBindVertexArray(0);
        }

        void PlaneGeometry::GenerateBuffer()
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(s_PlaneVertices), s_PlaneVertices, GL_STATIC_DRAW);
            glBindVertexArray(m_VAO);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

            glBindVertexArray(0);
        }

    }
}