#include "stdafx.h"

#include "Plane.h"

namespace Graphics
{
    namespace Geometry
    {
        const GLfloat Plane::s_PlaneVertices[] =
        {
            0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
        };

        Plane::Plane() :
            Geometry(mat4())
        {
            SetVertices();
        }

        Plane::Plane(const mat4& transform) :
            Geometry(transform)
        {
            SetVertices();
        }

        void Plane::Draw(bool wireframe) const
        {
            Bind();
            glDrawArrays(wireframe ? GL_LINES : GL_TRIANGLE_STRIP, 0, 6);
            Unbind();
        }

        void Plane::SetVertices()
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(s_PlaneVertices), s_PlaneVertices, GL_STATIC_DRAW);
            Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            Unbind();
        }
    }
}