#include "stdafx.h"

#include "Quad.h"

namespace Graphics
{
    namespace Geometry
    {
        const GLfloat Quad::s_QuadVertices[] =
        {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        Quad::Quad() :
            Geometry(mat4())
        {
            SetVertices();
        }

        Quad::Quad(const mat4& transform) :
            Geometry(transform)
        {
            SetVertices();
        }

        void Quad::Draw(bool wireframe) const
        {
            Bind();
            glDrawArrays(wireframe ? GL_LINES : GL_TRIANGLE_STRIP, 0, 4);
            Unbind();
        }

        void Quad::SetVertices()
        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(s_QuadVertices), s_QuadVertices, GL_STATIC_DRAW);
            Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            Unbind();
        }
    }
}