#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <vector>
#include "../../Core/Math/MathHelper.h"
#include "../Resource/Material.h"

namespace Graphics
{
    namespace Resource
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
            Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, const std::string& materialName) :
                m_Vertices(*new std::vector<Vertex>()),
                m_Indices(*new std::vector<GLuint>()),
                m_MaterialName(materialName)
            {
                m_Vertices = vertices;
                m_Indices = indices;
                InitMesh();
            }

            ~Mesh()
            {
            }

            void Draw(std::vector<Material*> material, bool wireframe) const
            {
                glBindVertexArray(m_VAO);
                glDrawElements(wireframe ? GL_LINES : GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }

            const std::string& GetMaterialName() const
            {
                return m_MaterialName;
            }

        private:
            std::vector<Vertex>& m_Vertices;
            std::vector<GLuint>& m_Indices;
            std::string m_MaterialName;

            GLuint m_VAO;
            GLuint m_VBO;
            GLuint m_EBO;

            void InitMesh()
            {
                glGenVertexArrays(1, &m_VAO);
                glGenBuffers(1, &m_VBO);
                glGenBuffers(1, &m_EBO);

                glBindVertexArray(m_VAO);
                glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
                glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(GLuint), &m_Indices[0], GL_STATIC_DRAW);

                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

                glBindVertexArray(0);
            }
        };

        class CAERULUS_GRAPHICS Model
        {
        public:
            Model(const std::string& path);
            ~Model();

            bool IsLoaded() const;

            void Draw(bool wireframe) const;
            void DrawMesh(bool wireframe, unsigned int mesh) const;

            const std::vector<Mesh>& GetMeshes() const;

            const std::vector<Material*>& GetMaterials() const;
            void SetMaterials(std::vector<Material*> materials);

        private:

            bool m_IsLoaded;
            std::string m_Path;
            std::vector<Mesh>& m_Meshes;
            std::vector<Material*> m_Materials;
        };
    }
}

namespace std
{
    template<>
    struct hash<Graphics::Resource::Vertex>
    {
        size_t operator()(const Graphics::Resource::Vertex& vertex) const
        {
            return
                ((hash<glm::vec3>()(vertex.Position) ^
                (hash<glm::vec3>()(vertex.Normal) << 1)) >> 1) ^
                    (hash<glm::vec2>()(vertex.TexCoords) << 1);
        }
    };
}