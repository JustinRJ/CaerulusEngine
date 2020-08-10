#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
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

        class CAERULUS_GRAPHICS Mesh : public Geometry
        {
        public:
            Mesh(const std::vector<Vertex>& vertices, const std::vector<GLuint>& indices, const std::string& materialName);
            virtual ~Mesh() = default;

            void Draw(bool wireframe) const override;
            const std::string& GetMaterialName() const;

        private:
            void SetVertices() override;

            const std::vector<Vertex> m_vertices;
            const std::vector<GLuint> m_indices;
            const std::string m_materialName;

            GLuint m_EBO;
        };
    }
}
