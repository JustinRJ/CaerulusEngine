#pragma once

#include "Node/Node.h"
#include "Managers/MaterialManager.h"
#include "Pipeline/ShaderUniformFunctor.h"

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
            Model(const Managers::ShaderManager& shaderManager, const std::string& path);

            bool IsLoaded() const;

            std::vector<std::string> GetMaterials() const;
            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

        private:
            void LoadModel(std::vector<std::vector<Geometry::Vertex>>& verticesOut, std::vector<std::vector<GLuint>>& indicesOut, std::vector<std::string>& materialNamesOut);
            void CalculateTangentAndBiTangent(std::vector<std::vector<Geometry::Vertex>>& vertices);

            bool m_isLoaded;
            const std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;

            const Managers::ShaderManager& m_shaderManager;
        };
    }
}