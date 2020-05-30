#include "stdafx.h"

#include "Model.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <tiny_obj_loader.h>
#include "Material.h"
#include "Graphics/Geometry/Mesh.h"

namespace
{
    using namespace Graphics::Geometry;
}

namespace std
{
    template<>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex& vertex) const
        {
            return
                ((hash<vec3>()(vertex.Position) ^
                (hash<vec3>()(vertex.Normal) << 1)) >> 1) ^
                    (hash<vec2>()(vertex.TexCoords) << 1);
        }
    };
}

namespace Graphics
{
    namespace Resource
    {
        Model::Model(
            const std::string& path) :
            m_isLoaded(false),
            m_path(path)
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
            for (const auto& shape : shapes)
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

                    if (materialName != prevMaterialName && prevMaterialName != "")
                    {
                        m_meshes.push_back(std::make_shared<Mesh>(vertices, indices, prevMaterialName));
                        vertices.clear();
                        indices.clear();
                    }

                    unsigned int fv = shape.mesh.num_face_vertices[f];
                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                        Vertex vertex;

                        vertex.Position = {
                            attrib.vertices[3 * idx.vertex_index + 0],
                            attrib.vertices[3 * idx.vertex_index + 1],
                            attrib.vertices[3 * idx.vertex_index + 2] };

                        vertex.TexCoords = {
                            attrib.texcoords[2 * idx.texcoord_index + 0],
                            attrib.texcoords[2 * idx.texcoord_index + 1] };

                        vertex.Normal = {
                            attrib.normals[3 * idx.normal_index + 0],
                            attrib.normals[3 * idx.normal_index + 1],
                            attrib.normals[3 * idx.normal_index + 2] };

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
                        m_meshes.push_back(std::make_shared<Mesh>(vertices, indices, materialName));
                    }
                    prevMaterialName = materialName;
                }
            }
            m_isLoaded = true;
        }

        void Model::Draw(bool wireframe, std::shared_ptr<Material> defaultMaterial) const
        {
            for (GLuint i = 0; i < m_meshes.size(); ++i)
            {
                if (i < m_materials.size())
                {
                    if (m_materials[i])
                    {
                        m_materials[i]->Bind();
                        DrawMesh(wireframe, i);
                    }
                    else if (defaultMaterial)
                    {
                        defaultMaterial->Bind();
                        DrawMesh(wireframe, i);
                    }
                }
            }
        }

        void Model::DrawMesh(bool wireframe, unsigned int mesh) const
        {
            m_meshes[mesh]->Draw(wireframe);
        }

        const std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes() const
        {
            return m_meshes;
        }

        bool Model::IsLoaded() const
        {
            return m_isLoaded;
        }

        const std::vector<std::shared_ptr<Material>>& Model::GetMaterials() const
        {
            return m_materials;
        }

        void Model::SetMaterials(std::vector<std::shared_ptr<Material>> materials)
        {
            m_materials = materials;
        }
    }
}
