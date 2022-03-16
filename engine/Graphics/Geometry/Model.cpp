#include "stdafx.h"

#include "Geometry/Model.h"

#include <tiny_obj_loader.h>

#include "Geometry/Mesh.h"
#include "Surface/Material.h"
#include "Rendering/GLRenderer.h"

using namespace Core::Math;
using namespace Graphics::Geometry;

template<>
struct std::hash<Vertex>
{
    size_t operator()(const Vertex& vertex) const
    {
        return
            ((hash<vec3>()(vertex.Position) ^
            (hash<vec3>()(vertex.Normal) << 1)) >> 1) ^
                (hash<vec2>()(vertex.TexCoord) << 1);
    }
};

namespace Graphics
{
    namespace Geometry
    {
        Model::Model(
            Core::ECS::Entity& entity,
            const std::string& path) :
            Core::ECS::Component(entity),
            m_isLoaded(false),
            m_path(path)
        {
            std::vector<std::vector<Vertex>> vertices;
            std::vector<std::vector<GLuint>> indices;
            std::vector<std::string> materialNames;

            LoadModel(vertices, indices, materialNames);
            //CalculateTangentAndBiTangent(vertices);

            for (unsigned int i = 0; i < vertices.size(); ++i)
            {
                std::shared_ptr<Mesh> newMesh =
                    std::make_shared<Mesh>(vertices.at(i), indices.at(i), materialNames.at(i));
                m_meshes.push_back(newMesh);
            }

            m_isLoaded = true;
        }

        void Model::Update(float deltaTime)
        {
            InvokeUniformCallbacks();
            for (const std::shared_ptr<Mesh>& mesh : GetMeshes())
            {
                if (mesh && mesh->GetRenderer())
                {
                    mesh->InvokeUniformCallbacks();
                    if (const Surface::Material* material = mesh->GetMaterial())
                    {
                        material->Bind();
                        material->InvokeUniformCallbacks();
                        mesh->Draw();
                    }
                }
            }
        }

        void Model::LoadModel(std::vector<std::vector<Vertex>>& verticesOut, std::vector<std::vector<GLuint>>& indicesOut, std::vector<std::string>& materialNamesOut)
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string error;

            std::string generalPath = m_path;
            generalPath.erase(m_path.find_last_of('/'));
            generalPath.append("/");

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, m_path.c_str(), generalPath.c_str()))
            {
                throw std::runtime_error(error);
            }

            std::unordered_map<Vertex, uint32_t> uniqueVertices;
            for (const tinyobj::shape_t& shape : shapes)
            {
                std::string materialName = "";
                std::string prevMaterialName = "";
                std::vector<Vertex> vertices;
                std::vector<GLuint> indices;
                // Loop over faces(polygon)
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
                {
                    int firstMeshesFaceMaterialID = shape.mesh.material_ids[f];
                    if (firstMeshesFaceMaterialID != -1)
                    {
                        materialName = materials[firstMeshesFaceMaterialID].name;
                    }

                    if (f != 0 && materialName != prevMaterialName)
                    {
                        verticesOut.push_back(vertices);
                        indicesOut.push_back(indices);
                        materialNamesOut.push_back(prevMaterialName);
                        vertices.clear();
                        indices.clear();
                    }

                    unsigned int fv = shape.mesh.num_face_vertices[f];
                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                        Vertex vertex;

                        Core::Math::vec3 position(
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2]);

                        Core::Math::vec2 texCoord(
                            attrib.texcoords[2 * idx.texcoord_index + 0],
                            attrib.texcoords[2 * idx.texcoord_index + 1]);

                        Core::Math::vec3 normal(
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2]);

                        vertex.Position = position;
                        vertex.TexCoord = texCoord;
                        vertex.Normal = normal;

                        if (uniqueVertices.count(vertex) == 0)
                        {
                            uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                            vertices.push_back(vertex);
                        }
                        indices.push_back(uniqueVertices[vertex]);
                    }
                    index_offset += fv;

                    if (f == shape.mesh.num_face_vertices.size() - 1)
                    {
                        verticesOut.push_back(vertices);
                        indicesOut.push_back(indices);
                        materialNamesOut.push_back(materialName);
                        vertices.clear();
                        indices.clear();
                    }
                    prevMaterialName = materialName;
                }
            }
        }

        void Model::CalculateTangentAndBiTangent(std::vector<std::vector<Vertex>>& vertices)
        {
            for (std::vector<Vertex>& meshVertices : vertices)
            {
                for (unsigned int i = 0; i < meshVertices.size() - 2; i += 3)
                {
                    Vertex v1 = meshVertices.at(i);
                    Vertex v2 = meshVertices.at(i + 1);
                    Vertex v3 = meshVertices.at(i + 2);

                    vec3 edge1 = v2.Position - v1.Position;
                    vec3 edge2 = v3.Position - v1.Position;

                    vec2 deltaUV1 = v2.TexCoord - v1.TexCoord;
                    vec2 deltaUV2 = v3.TexCoord - v1.TexCoord;

                    GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    vec3 tangent;
                    tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangent = normalize(tangent);

                    vec3 biTangent;
                    biTangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    biTangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    biTangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    biTangent = normalize(biTangent);
                }
            }
        }

        const std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes() const
        {
            return m_meshes;
        }

        bool Model::IsLoaded() const
        {
            return m_isLoaded;
        }
    }
}
