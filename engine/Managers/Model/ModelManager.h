#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "../Manager.h"
#include "../Material/MaterialManager.h"
#include "../../Graphics/Resource/Model.h"

namespace Managers
{
    namespace Model
    {
        class CAERULUS_MANAGERS ModelManager : public Manager<Graphics::Resource::Model>
        {
        public:
            ModelManager(Material::MaterialManager& materialManager);
            virtual ~ModelManager() {}

            bool Load(const std::string& name, const std::string& modelPath, const std::string& materialPath = "");

        private:
            Material::MaterialManager& m_MaterialManager;
        };
    }
}