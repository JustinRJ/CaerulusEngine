#pragma once

#define CAERULUS_CORE __declspec(dllexport)

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
        class CAERULUS_CORE ModelManager : public Core::Template::Manager<Model>
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