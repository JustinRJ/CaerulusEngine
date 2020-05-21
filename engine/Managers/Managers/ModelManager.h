#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "Managers/Manager.h"
#include "MaterialManager.h"
#include "Graphics/Resource/Model.h"

namespace Managers
{
    using namespace Graphics::Resource;
    class CAERULUS_MANAGERS ModelManager : public Manager<Model>
    {
    public:
        ModelManager(MaterialManager& materialManager);
        virtual ~ModelManager() = default;

        bool Load(const std::string& name, const std::string& modelPath, const std::string& materialPath = "");

    private:
        MaterialManager& m_materialManager;
    };
}