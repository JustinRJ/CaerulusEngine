#include "stdafx.h"

#include "ModelManager.h"
#include "Graphics/Geometry/Mesh.h"

namespace Managers
{
    ModelManager::ModelManager(MaterialManager& materialManager) :
        m_materialManager(materialManager)
    {
    }

    void ModelManager::Load(const std::string& name, const std::string& modelPath, const std::string& materialPath)
    {
        if (IsLoaded(name))
        {
            Log::LogInDebug("Model with name " + name + " already loaded with path: " + modelPath);
            return;
        }

        Log::LogMessage("Loading model " + name + " with path: " + modelPath);
        std::shared_ptr<Model> newModel = std::make_shared<Model>(modelPath);
        if (!newModel->IsLoaded())
        {
            Log::LogInDebug("Model with name " + name + " failed to load with path: " + modelPath);
            return;
        }

        std::string tempMaterialPath(materialPath);
        if (tempMaterialPath == "")
        {
            tempMaterialPath = modelPath;
            tempMaterialPath.erase(tempMaterialPath.find_last_of('.'));
            tempMaterialPath.append(".mtl");
        }

        std::vector<std::shared_ptr<Material>> newModelMaterials;
        m_materialManager.Load(tempMaterialPath);

        for (std::shared_ptr<Graphics::Geometry::Mesh> mesh : newModel->GetMeshes())
        {
            newModelMaterials.push_back(m_materialManager.Get(mesh->GetMaterialName()));
        }

        newModel->SetMaterials(newModelMaterials);

        Insert(name, newModel);
    }
}