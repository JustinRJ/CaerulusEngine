#include "stdafx.h"

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        Geometry::Geometry() :
            m_Transform(mat4())
        {
            GenerateBuffer();
        }

        Geometry::Geometry(const mat4& transform) :
            m_Transform(mat4(transform))
        {
            GenerateBuffer();
        }

        void Geometry::GenerateBuffer()
        {
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        }

        GLuint Geometry::GetVAO() const
        {
            return m_VAO;
        }

        GLuint Geometry::GetVBO() const
        {
            return m_VBO;
        }

        void Geometry::SetTransform(const mat4& transform)
        {
            m_Transform = transform;
        }

        const mat4& Geometry::GetTransform() const
        {
            return m_Transform;
        }
    }
}