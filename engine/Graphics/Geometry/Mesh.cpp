#include "stdafx.h"

#include "Mesh.h"

namespace Graphics
{
    namespace Geometry
    {
        Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, const std::string& materialName) :
            m_Vertices(*new std::vector<Vertex>()),
            m_Indices(*new std::vector<GLuint>()),
            m_MaterialName(materialName)
        {
            m_Vertices = vertices;
            m_Indices = indices;
            SetVertices();
        }

        void Mesh::Draw(bool wireframe) const
        {
            Bind();
            glDrawElements(wireframe ? GL_LINES : GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
            Unbind();
        }

        const std::string& Mesh::GetMaterialName() const
        {
            return m_MaterialName;
        }

        void Mesh::SetVertices()
        {
            Bind();
            glGenBuffers(1, &m_EBO);
            glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
            Unbind();
        }
    };
}