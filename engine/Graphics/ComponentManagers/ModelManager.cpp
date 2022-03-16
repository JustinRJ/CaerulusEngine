#include "stdafx.h"

#include "ComponentManagers/ModelManager.h"
#include "AssetManagers/MaterialManager.h"

#include "ECS/Entity.h"
#include "Geometry/Mesh.h"

using namespace Core::Logging;
using namespace Graphics::Geometry;

namespace Graphics
{
    namespace Managers
    {
        ModelManager::ModelManager(MaterialManager& materialManager, const Rendering::GLRenderer* renderer) :
            m_materialManager(materialManager),
            m_renderer(renderer)
        {}

        const Geometry::Model* ModelManager::Load(Core::ECS::Entity& entity, const std::string& modelPath, const std::string& materialPath)
        {
            const Geometry::Model* ptr = nullptr;
            if (auto loadedModel = Get(&entity))
            {
                ptr = loadedModel;
                LogInDebug("Model with name " + entity.GetName() + " already loaded with path: " + modelPath);
            }
            else
            {
                LogMessage("Loading model " + entity.GetName() + " with path: " + modelPath);
                std::unique_ptr<Model> newModel = std::make_unique<Model>(entity, modelPath);
                if (!newModel->IsLoaded())
                {
                    LogInDebug("Model with name " + entity.GetName() + " failed to load with path: " + modelPath);
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
                    for (auto& mesh : newModel->GetMeshes())
                    {
                        mesh->SetRenderer(m_renderer);
                    }
                    ptr = newModel.get();
                    Insert(&entity, std::move(newModel));
                }
            }
            return ptr;
        }

        MaterialManager& ModelManager::GetMaterialManager()
        {
            return m_materialManager;
        }

        void ModelManager::AddModelUniformCallback(Core::ECS::Entity& key, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback)
        {
            if (Model* model = GetMutable(&key))
            {
                model->AddUniformCallback(shader, uniformCallback);
            }
        }
    }
}