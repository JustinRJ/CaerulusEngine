#include "stdafx.h"

#include "Model.h"
#include <iostream>

namespace Graphics
{
    namespace Resource
    {
        Model::Model(
            const std::string& path) :
            m_IsLoaded(false),
            m_Path(*new std::string(path)),
            m_Meshes(*new std::vector<Mesh>()),
            m_MeshMaterials(*new std::vector<std::string>())
        {
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(m_Path,
                aiProcess_Triangulate |
                aiProcess_FlipUVs |
                aiProcess_CalcTangentSpace |
                aiProcess_OptimizeGraph |
                aiProcess_SplitLargeMeshes |
                aiProcess_OptimizeMeshes |
                aiProcess_SortByPType |
                aiProcess_OptimizeGraph |
                aiProcess_FindInstances);

            if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
                return;
            }

            ProcessNode(scene->mRootNode, scene);

            m_IsLoaded = true;
        }

        Model::~Model()
        {
        }

        void Model::Draw(bool wireframe) const
        {
            for (GLuint i = 0; i < m_Meshes.size(); i++)
            {
                DrawMesh(wireframe, i);
            }
        }

        void Model::DrawMesh(bool wireframe, unsigned int mesh) const
        {
            m_Meshes[mesh].Draw(wireframe);
        }

        void Model::ProcessNode(const aiNode* node, const aiScene* scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                //pushback mesh
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_Meshes.push_back(ProcessMesh(*mesh));

                //pushback mesh name
                aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
                aiString materialName;
                mat->Get(AI_MATKEY_NAME, materialName);
                if (!std::string("DefaultMaterial").compare(materialName.C_Str()))
                {
                    m_MeshMaterials.push_back("");
                }
                else
                {
                    m_MeshMaterials.push_back(materialName.C_Str());
                }
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene);
            }
        }

        const Mesh& Model::ProcessMesh(const aiMesh& mesh) const
        {
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;

            for (unsigned int i = 0; i < mesh.mNumVertices; i++)
            {
                Vertex vertex;
                vec3 vector;

                vector.x = mesh.mVertices[i].x;
                vector.y = mesh.mVertices[i].y;
                vector.z = mesh.mVertices[i].z;
                vertex.Position = vector;

                vector.x = mesh.mNormals[i].x;
                vector.y = mesh.mNormals[i].y;
                vector.z = mesh.mNormals[i].z;
                vertex.Normal = vector;

                if (mesh.mTextureCoords[0])
                {
                    vec2 vec;
                    vec.x = mesh.mTextureCoords[0][i].x;
                    vec.y = mesh.mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                }
                else
                {
                    vertex.TexCoords = vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh.mNumFaces; i++)
            {
                aiFace face = mesh.mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                {
                    indices.push_back(face.mIndices[j]);
                }
            }

            return *new Mesh(vertices, indices);
        }

        const std::vector<Mesh>& Model::GetMeshes() const
        {
            return m_Meshes;
        }

        const std::vector<std::string>& Model::GetMeshMaterials() const
        {
            return m_MeshMaterials;
        }

        bool Model::IsLoaded() const
        {
            return m_IsLoaded;
        }
    }
}
