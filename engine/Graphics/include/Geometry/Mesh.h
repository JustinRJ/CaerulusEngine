#pragma once

#include "GPUGeometry.h"

namespace Graphics
{
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

        class Mesh : public GPUGeometry
        {
        public:
            Mesh(Core::Node::Node* parent,
                const Managers::ShaderManager& shaderManager,
                const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& fileMaterialName) :
                GPUGeometry(parent, shaderManager, &vertices[0], sizeof(Vertex)* vertices.size(), &indices[0], indices.size()),
                m_vertices(vertices),
                m_indices(indices),
                m_fileMaterialName(fileMaterialName)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                layout.Push<float>(3);
                layout.Push<float>(2);
                GetVertexArray().AddBuffer(GetVertexBuffer(), layout);
            }

            const std::string& GetFileMaterialName() const
            {
                return m_fileMaterialName;
            }

        private:
            const std::vector<Vertex> m_vertices;
            const std::vector<GLuint> m_indices;
            const std::string m_fileMaterialName;
        };
    }
}
