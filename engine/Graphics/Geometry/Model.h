#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        class Mesh;

        class CAERULUS_GRAPHICS Model : public Core::Interface::NonCopyable
        {
        public:
            Model(const std::string& path);
            virtual ~Model() = default;

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

            std::vector<std::shared_ptr<Surface::Material>> GetMaterials() const;

        private:
            bool m_isLoaded;
            std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
        };
    }
}