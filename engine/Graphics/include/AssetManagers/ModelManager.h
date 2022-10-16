#pragma once

#include "MaterialManager.h"
#include "Geometry/Model.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS ModelManager : public Core::ECS::AssetManager<Geometry::Model>
        {
        public:
            ModelManager(MaterialManager& materialManager, Rendering::IRenderer* renderer);

            void Load(std::string_view name, std::string_view path, std::string_view materialPath = "");

        private:
            MaterialManager& m_materialManager;
            Rendering::IRenderer* m_renderer;
        };
    }
}