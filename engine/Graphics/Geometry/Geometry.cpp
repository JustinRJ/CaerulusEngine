#include "stdafx.h"

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace Core::Math;
        Geometry::Geometry(const glm::mat4& transform)
            : m_Transform(*new glm::mat4(transform))
        {
            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        }

        Geometry::~Geometry()
        {
        }

        GLuint Geometry::GetVAO() const
        {
            return m_VAO;
        }

        GLuint Geometry::GetVBO() const
        {
            return m_VBO;
        }

        void Geometry::SetTransform(glm::mat4& transform)
        {
            m_Transform = transform;
        }

        glm::mat4& Geometry::GetTransform() const
        {
            return m_Transform;
        }
        }
}