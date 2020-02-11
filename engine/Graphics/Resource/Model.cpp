#include "stdafx.h"

#include "Model.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <tiny_obj_loader.h>

namespace Graphics
{
    namespace Resource
    {
        Model::Model(
            const std::string& path) :
            m_IsLoaded(false),
            m_Path(path),
            m_Meshes(*new std::vector<Mesh>()),
            m_Materials(std::vector<std::shared_ptr<Material>>())
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string error;

            std::string generalPath = m_Path;
            generalPath.erase(m_Path.find_last_of('/'));
            generalPath.append("/");

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, m_Path.c_str(), generalPath.c_str()))
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
                        materialName = materials.at(firstMeshesFaceMaterialID).name;
                    }

                    if (materialName != prevMaterialName && prevMaterialName != "")
                    {
                        m_Meshes.push_back(Mesh(vertices, indices, prevMaterialName));
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
                        m_Meshes.push_back(Mesh(vertices, indices, materialName));
                    }
                    prevMaterialName = materialName;
                }
            }
            m_IsLoaded = true;
        }

        Model::~Model()
        {
        }

        void Model::Draw(bool wireframe, std::shared_ptr<Material> defaultMaterial) const
        {
            for (GLuint i = 0; i < m_Meshes.size(); ++i)
            {
                if (i < m_Materials.size())
                {
                    if (m_Materials.at(i))
                    {
                        m_Materials.at(i)->Bind();
                        DrawMesh(wireframe, i);
                    }
                    else if(defaultMaterial)
                    {
                        defaultMaterial->Bind();
                        DrawMesh(wireframe, i);
                    }
                }
            }
        }

        void Model::DrawMesh(bool wireframe, unsigned int mesh) const
        {
            m_Meshes.at(mesh).Draw(wireframe);
        }

        const std::vector<Mesh>& Model::GetMeshes() const
        {
            return m_Meshes;
        }

        bool Model::IsLoaded() const
        {
            return m_IsLoaded;
        }

        const std::vector<std::shared_ptr<Material>>& Model::GetMaterials() const
        {
            return m_Materials;
        }

        void Model::SetMaterials(std::vector<std::shared_ptr<Material>> materials)
        {
            m_Materials = materials;
        }
    }
}
