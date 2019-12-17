#include "stdafx.h"

#include "ModelManager.h"
#include <iostream>

namespace Managers
{
    namespace Model
    {
        ModelManager::ModelManager(Material::MaterialManager& materialManager) :
            m_MaterialManager(materialManager)
        {
        }

        bool ModelManager::Load(const std::string& name, const std::string& modelPath, const std::string& materialPath)
        {
            if (IsLoaded(name))
            {
                return false;
            }
            std::cout << "Loading model " + name + " with path " + modelPath << std::endl;

            Graphics::Resource::Model* newModel = new Graphics::Resource::Model(modelPath);

            std::string tempMaterialPath(materialPath);
            if (tempMaterialPath == "")
            {
                tempMaterialPath = modelPath;
                tempMaterialPath.erase(tempMaterialPath.find_last_of('.'));
                tempMaterialPath.append(".mtl");
            }

            std::vector<Graphics::Resource::Material*> newModelMaterials;
            if (m_MaterialManager.Load(tempMaterialPath))
            {
                for (auto mesh : newModel->GetMeshes())
                {
                    newModelMaterials.push_back(m_MaterialManager.Get(mesh.GetMaterialName()));
                }
            }
            newModel->SetMaterials(newModelMaterials);

            Insert(name, newModel);
            return true;
        }
    }
}