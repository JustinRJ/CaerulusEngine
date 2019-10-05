#include "stdafx.h"

#include "Model.h"
#include <iostream>
#include <unordered_map>

// define this in only *one* .cpp
#define TINYOBJLOADER_IMPLEMENTATION 
#include <tiny_obj_loader.h>

namespace Graphics
{
    namespace Resource
    {
        Model::Model(
            const std::string& path) :
            m_IsLoaded(false),
            m_Path(*new std::string(path)),
            m_Meshes(*new std::vector<Mesh>()),
            m_Materials()
        {
            tinyobj::attrib_t attrib;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string error;

            if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &error, m_Path.c_str()))
                throw std::runtime_error(error);

            // std::unordered_map<Vertex, uint32_t> uniqueVertices;
            for (const auto& shape : shapes)
            {
                std::vector<Vertex> vertices;
                std::vector<GLuint> indices;
                // Loop over faces(polygon)
                size_t index_offset = 0;
                for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
                {
                    unsigned int fv = shape.mesh.num_face_vertices[f];
                    for (size_t v = 0; v < fv; ++v)
                    {
                        tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

                        Vertex vertex = {};

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

                        //if (uniqueVertices.count(vertex) == 0)
                        //{
                        //    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                        //    vertices.push_back(vertex);
                        //}
                        vertices.push_back(vertex);
                        indices.push_back(indices.size());
                    }

                    index_offset += fv;
                    // per-face material
                    shape.mesh.material_ids[f];
                }
                m_Meshes.push_back(Mesh(vertices, indices));
            }
            m_IsLoaded = true;
        }

        Model::~Model()
        {
        }

        void Model::Draw(bool wireframe) const
        {
            for (GLuint i = 0; i < m_Meshes.size(); i++)
            {
                if (m_Materials.size() > i && m_Materials.at(i))
                {
                    m_Materials.at(i)->RenderToShader();
                }
                DrawMesh(wireframe, i);
            }
        }

        void Model::DrawMesh(bool wireframe, unsigned int mesh) const
        {
            m_Meshes[mesh].Draw(wireframe);
        }

        const std::vector<Mesh>& Model::GetMeshes() const
        {
            return m_Meshes;
        }

        bool Model::IsLoaded() const
        {
            return m_IsLoaded;
        }

        const std::vector<Material*>& Model::GetMaterials() const
        {
            return m_Materials;
        }

        void Model::SetMaterials(const std::vector<Material*>& materials)
        {
            m_Materials = materials;
        }
    }
}
