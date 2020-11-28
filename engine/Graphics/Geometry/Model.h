#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        class Mesh;

        class CAERULUS_GRAPHICS Model : public Pipeline::ShaderUniformFunctor
        {
        public:
            Model(const std::string& path);
            ~Model() = default;

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;
            std::vector<std::shared_ptr<Surface::Material>> GetMaterials() const;

        private:
            bool m_isLoaded;
            const std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
        };
    }
}