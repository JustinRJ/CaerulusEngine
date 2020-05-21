#include "stdafx.h"

#include "ModelManager.h"
#include <iostream>

namespace Managers
{
    ModelManager::ModelManager(MaterialManager& materialManager) :
        m_materialManager(materialManager)
    {
    }

    bool ModelManager::Load(const std::string& name, const std::string& modelPath, const std::string& materialPath)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Model with name " + name + " already loaded with path " + modelPath);
            return false;
        }

        using namespace Core::Logging;
        Log::LogMessage("Loading model " + name + " with path " + modelPath);
        auto newModel = std::make_shared<Model>(modelPath);
        std::string tempMaterialPath(materialPath);
        if (tempMaterialPath == "")
        {
            tempMaterialPath = modelPath;
            tempMaterialPath.erase(tempMaterialPath.find_last_of('.'));
            tempMaterialPath.append(".mtl");
        }

        std::vector<std::shared_ptr<Material>> newModelMaterials;
        if (m_materialManager.Load(tempMaterialPath))
        {
            for (auto& mesh : newModel->GetMeshes())
            {
                newModelMaterials.push_back(m_materialManager.Get(mesh->GetMaterialName()));
            }
        }
        newModel->SetMaterials(newModelMaterials);

        Insert(name, newModel);
        return true;
    }
}