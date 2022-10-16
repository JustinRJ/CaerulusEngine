#pragma once

#include "Geometry/Mesh.h"

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
    namespace Rendering
    {
        class IRenderer;
    }

    namespace AssetManagers
    {
        class MaterialManager;
    }

    namespace Geometry
    {
        class Mesh;

        class CAERULUS_GRAPHICS Model
        {
        public:
            Model();

            void Load(std::string_view modelPath, Rendering::IRenderer* renderer, AssetManagers::MaterialManager* materialManager = nullptr, std::string_view materialPath = "");

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

        private:
            void LoadModel(std::vector<std::vector<Geometry::Vertex>>& verticesOut, std::vector<std::vector<GLuint>>& indicesOut, std::vector<std::string>& materialNamesOut);
            void CalculateTangentAndBiTangent(std::vector<std::vector<Geometry::Vertex>>& vertices);

            bool m_isLoaded;
            std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
        };
    }
}