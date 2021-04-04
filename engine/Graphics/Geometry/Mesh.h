#pragma once

#include "Core/Math/Math.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        struct Vertex
        {
            Core::Math::vec3 Position;
            Core::Math::vec3 Normal;
            Core::Math::vec2 TexCoord;

            bool operator==(const Vertex& other) const
            {
                return
                    Position == other.Position &&
                    Normal == other.Normal &&
                    TexCoord == other.TexCoord;
            }
        };

        class Mesh : public Pipeline::ShaderUniformFunctor
        {
        public:
            Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& materialName) :
                m_vertexBuffer(&m_vertices[0], sizeof(Vertex) * m_vertices.size()),
                m_indexBuffer(&m_indices[0], m_indices.size()),
                m_vertices(vertices),
                m_indices(indices),
                m_materialName(materialName)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                layout.Push<float>(3);
                layout.Push<float>(2);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Mesh() = default;

            const std::string& GetMaterialName() const
            {
                return m_materialName;
            }

            const Pipeline::VertexArray& GetVertexArray() const
            {
                return m_vertexArray;
            }

            const Pipeline::VertexBuffer& GetVertexBuffer() const
            {
                return m_vertexBuffer;
            }

            const Pipeline::IndexBuffer& GetIndexBuffer() const
            {
                return m_indexBuffer;
            }

            std::shared_ptr<Surface::Material> GetMaterial() const
            {
                return m_material;
            }

            void SetMaterial(std::shared_ptr<Surface::Material> material)
            {
                m_material = material;
            }

        private:
            const std::vector<Vertex> m_vertices;
            const std::vector<GLuint> m_indices;
            const std::string m_materialName;

            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;

            std::shared_ptr<Surface::Material> m_material;
        };
    }
}
