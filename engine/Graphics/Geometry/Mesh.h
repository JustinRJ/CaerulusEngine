#pragma once

#include "Core/Math/Math.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace Core::Math;
        struct Vertex
        {
            vec3 Position;
            vec3 Normal;
            vec2 TexCoords;

            bool operator==(const Vertex& other) const
            {
                return
                    Position == other.Position &&
                    Normal == other.Normal &&
                    TexCoords == other.TexCoords;
            }
        };

        class Mesh
        {
        public:
            Mesh(const mat4& transform, const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& materialName) :
                m_vertexBuffer(&m_vertices[0], sizeof(Vertex) * m_vertices.size()),
                m_indexBuffer(&m_indices[0], m_indices.size()),
                m_vertices(vertices),
                m_indices(indices),
                m_materialName(materialName)
            {
                PipeLine::VertexBufferLayout layout;
                layout.Push<float>(3);
                layout.Push<float>(3);
                layout.Push<float>(2);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Mesh() = default;

            void Draw(bool wireframe) const
            {
                m_vertexArray.Bind();
                m_indexBuffer.Bind();
                glDrawElements(wireframe ? GL_LINE_LOOP : GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, nullptr);
            }

            const std::string& GetMaterialName() const
            {
                return m_materialName;
            }

            const PipeLine::VertexArray& GetVertexArray() const
            {
                return m_vertexArray;
            }

            const PipeLine::VertexBuffer& GetVertexBuffer() const
            {
                return m_vertexBuffer;
            }

            const PipeLine::IndexBuffer& GetIndexBuffer() const
            {
                return m_indexBuffer;
            }

        private:
            const std::vector<Vertex> m_vertices;
            const std::vector<GLuint> m_indices;
            const std::string m_materialName;

            PipeLine::VertexArray m_vertexArray;
            PipeLine::VertexBuffer m_vertexBuffer;
            PipeLine::IndexBuffer m_indexBuffer;
        };
    }
}
