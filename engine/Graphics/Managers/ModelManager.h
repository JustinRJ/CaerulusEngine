#pragma once


#include "MaterialManager.h"
#include "Graphics/Geometry/Model.h"

namespace
{
    using namespace Graphics::Geometry;
}

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ModelManager : public Core::Template::Manager<Model>
        {
        public:
            ModelManager(MaterialManager& materialManager);
            ~ModelManager() = default;

            void Load(const std::string& name, const std::string& modelPath, const std::string& materialPath = "");

        private:
            MaterialManager& m_materialManager;
        };
    }
}