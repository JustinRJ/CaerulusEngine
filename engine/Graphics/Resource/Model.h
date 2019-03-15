#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glew.h>
#include <vector>
#include "../../Core/Math/MathFacade.h"
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
        };

        class Mesh
        {
        public:
            Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) :
                m_Indices(*new std::vector<GLuint>()),
                m_Vertices(*new std::vector<Vertex>())
            {
                m_Vertices = vertices;
                m_Indices = indices;
                InitMesh();
            }

            ~Mesh()
            {
            }

            void Draw(bool wireframe) const
            {
                glBindVertexArray(m_VAO);
                glDrawElements(wireframe ? GL_LINES : GL_TRIANGLES, (GLsizei)m_Indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }

        private:
            std::vector<Vertex>& m_Vertices;
            std::vector<GLuint>& m_Indices;

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

        class Model
        {
        public:
            CAERULUS_GRAPHICS Model(const std::string& path);
            CAERULUS_GRAPHICS ~Model();

            CAERULUS_GRAPHICS bool IsLoaded() const;

            CAERULUS_GRAPHICS void Draw(bool wireframe) const;
            CAERULUS_GRAPHICS void DrawMesh(bool wireframe, unsigned int mesh) const;

            CAERULUS_GRAPHICS const std::vector<Mesh>& GetMeshes() const;
            CAERULUS_GRAPHICS const std::vector<std::string>& GetMeshMaterials() const;

            CAERULUS_GRAPHICS const std::vector<Material*>& GetMaterials() const;
            CAERULUS_GRAPHICS void SetMaterials(const std::vector<Material*>& materials);


        private:
            bool m_IsLoaded;
            std::string& m_Path;
            std::vector<Mesh>& m_Meshes;
            std::vector<std::string>& m_MeshMaterials;
            std::vector<Material*> m_Materials;

            void ProcessNode(const aiNode* node, const aiScene* scene);
            const Mesh& ProcessMesh(const aiMesh& mesh) const;
        };
    }
}