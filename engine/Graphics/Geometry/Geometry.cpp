#include "stdafx.h"

#include "Geometry.h"

namespace Caerulus
{
    namespace Render
    {
        namespace Geometry
        {
            using namespace Core::Math;
            Geometry::Geometry(const glm::vec3& scale, const glm::vec3& rot, const glm::vec3& translation) :
                m_Model(*new glm::mat4())
            {
                MathHelper::CreateTansform(m_Model, translation, rot, scale);

                glGenVertexArrays(1, &m_VAO);
                glGenBuffers(1, &m_VBO);
                glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            }

            Geometry::Geometry(glm::mat4& transform)
                : m_Model(*new glm::mat4(transform))
            {

            }

            Geometry::~Geometry()
            {
            }

            glm::vec3 Geometry::GetPosition() const
            {
                return MathHelper::GetTranslation(m_Model);
            }

            glm::quat Geometry::GetRotation() const
            {
                return MathHelper::GetRotation(m_Model);
            }

            glm::vec3 Geometry::GetScale() const
            {
                return MathHelper::GetScale(m_Model);
            }

            GLuint Geometry::GetVAO() const
            {
                return m_VAO;
            }

            GLuint Geometry::GetVBO() const
            {
                return m_VBO;
            }

            void Geometry::SetPosition(const glm::vec3& position)
            {
                MathHelper::SetTranslation(m_Model, position);
            }

            void Geometry::SetRotation(const glm::quat& rotation)
            {
                MathHelper::SetRotation(m_Model, rotation);
            }

            void Geometry::Scale(const glm::vec3&scale)
            {
                MathHelper::Scale(m_Model, scale);
            }
        }
    }
}