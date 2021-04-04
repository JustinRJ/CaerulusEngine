#pragma once

#include "Core/Node/Node.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        struct Vertex;
    }

    namespace Geometry
    {
        class Mesh;

        class CAERULUS_GRAPHICS Model : public Core::Node::Node, public Pipeline::ShaderUniformFunctor
        {
        public:
            Model(const std::string& path);
            ~Model() = default;

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;
            std::vector<std::shared_ptr<Surface::Material>> GetMaterials() const;

        private:
            void LoadModel(std::vector<std::vector<Geometry::Vertex>>& verticesOut, std::vector<std::vector<GLuint>>& indicesOut, std::vector<std::string>& materialNamesOut);
            //void CalculateTangentAndBiTangent(std::vector<std::vector<Geometry::Vertex>>& vertices);

            bool m_isLoaded;
            const std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
        };
    }
}