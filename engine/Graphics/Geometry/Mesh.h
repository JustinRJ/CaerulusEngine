#pragma once

#include <vector>
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
            static const GLfloat s_cubeVertices[];

            Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, const std::string& materialName);
            virtual ~Mesh() = default;

            void Draw(bool wireframe) const override;
            const std::string& GetMaterialName() const;

        private:
            void SetVertices() override;

            std::vector<Vertex>& m_vertices;
            std::vector<GLuint>& m_indices;
            std::string m_materialName;

            GLuint m_EBO;
        };
    }
}