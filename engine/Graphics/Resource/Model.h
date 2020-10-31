#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Geometry
    {
        class Mesh;
    }
    namespace Resource
    {
        class Material;
    }

    namespace Resource
    {
        class CAERULUS_GRAPHICS Model : public Core::Interface::NonCopyable
        {
        public:
            Model(const std::string& path);
            virtual ~Model() = default;

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Geometry::Mesh>>& GetMeshes() const;

            const std::vector<std::shared_ptr<Material>>& GetMaterials() const;
            void SetMaterials(std::vector<std::shared_ptr<Material>> materials);

        private:
            bool m_isLoaded;
            std::string m_path;
            std::vector<std::shared_ptr<Material>> m_materials;
            std::vector<std::shared_ptr<Geometry::Mesh>> m_meshes;
        };
    }
}