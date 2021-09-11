#include "stdafx.h"

#include "Managers/ModelManager.h"

#include "Geometry/Mesh.h"

using namespace Core::Logging;
using namespace Graphics::Geometry;

namespace Graphics
{
    namespace Managers
    {
        ModelManager::ModelManager(MaterialManager& materialManager) :
            m_materialManager(materialManager)
        {}

        void ModelManager::Load(const std::string& modelName, const std::string& modelPath, const std::string& materialPath)
        {
            if (IsLoaded(modelName))
            {
                LogInDebug("Model with name " + modelName + " already loaded with path: " + modelPath);
            }
            else
            {
                LogMessage("Loading model " + modelName + " with path: " + modelPath);
                std::unique_ptr<Model> newModel = std::make_unique<Model>(m_materialManager.GetShaderManager(), modelPath);
                if (!newModel->IsLoaded())
                {
                    LogInDebug("Model with name " + modelName + " failed to load with path: " + modelPath);
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

                    for (const std::shared_ptr<Mesh>& mesh : newModel->GetMeshes())
                    {
                        mesh->SetMaterial(mesh->GetMaterialName());
                    }

                    Insert(modelName, std::move(newModel));
                }
            }
        }

        MaterialManager& ModelManager::GetMaterialManager()
        {
            return m_materialManager;
        }

        void ModelManager::AddModelUniformFunctor(const std::string& modelName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformFunctor)
        {
            if (Model* material = GetMutable(modelName))
            {
                material->AddUniformFunctor(shaderName, uniformFunctor);
            }
        }

        void ModelManager::SetModelTransform(const std::string& modelName, const Core::Math::mat4& transform)
        {
            if (Model* material = GetMutable(modelName))
            {
                material->GetTransform().SetMatrix(transform);
            }
        }
    }
}