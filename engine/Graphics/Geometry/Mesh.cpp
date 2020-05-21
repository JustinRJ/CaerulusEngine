#include "stdafx.h"

#include "Mesh.h"

namespace Graphics
{
    namespace Geometry
    {
        Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& materialName) :
            m_vertices(vertices),
            m_indices(indices),
            m_materialName(materialName)
        {
            SetVertices();
        }

        void Mesh::Draw(bool wireframe) const
        {
            Bind();
            glDrawElements(wireframe ? GL_LINES : GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
            Unbind();
        }

        const std::string& Mesh::GetMaterialName() const
        {
            return m_materialName;
        }

        void Mesh::SetVertices()
        {
            Bind();
            glGenBuffers(1, &m_EBO);
            glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);

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