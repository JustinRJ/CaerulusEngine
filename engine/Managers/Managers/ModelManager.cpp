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
        }
        else
        {
            Log::LogMessage("Loading model " + name + " with path: " + modelPath);
            std::shared_ptr<Model> newModel = std::make_shared<Model>(modelPath);
            if (!newModel->IsLoaded())
            {
                Log::LogInDebug("Model with name " + name + " failed to load with path: " + modelPath);
            }
            else
            {
                std::string appendedMaterialPath(materialPath);
                if (appendedMaterialPath == "")
                {
                    appendedMaterialPath = modelPath;
                    appendedMaterialPath.erase(appendedMaterialPath.find_last_of('.'));
                    appendedMaterialPath.append(".mtl");
                }

                m_materialManager.Load(appendedMaterialPath);

                for (std::shared_ptr<Graphics::Geometry::Mesh> mesh : newModel->GetMeshes())
                {
                    mesh->SetMaterial(m_materialManager.Get(mesh->GetMaterialName()));
                }

                Insert(name, newModel);
            }
        }

    }
}