#include "stdafx.h"

#include "ComponentManagers/ModelManager.h"

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

        const Geometry::Model* ModelManager::Load(Core::Node::Node& node, const std::string& modelPath, const std::string& materialPath)
        {
            const Geometry::Model* ptr = nullptr;
            if (auto loadedModel = Get(node))
            {
                ptr = loadedModel;
                LogInDebug("Model with name " + node.GetName() + " already loaded with path: " + modelPath);
            }
            else
            {
                LogMessage("Loading model " + node.GetName() + " with path: " + modelPath);
                std::unique_ptr<Model> newModel = std::make_unique<Model>(node, modelPath);
                if (!newModel->IsLoaded())
                {
                    LogInDebug("Model with name " + node.GetName() + " failed to load with path: " + modelPath);
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
                    ptr = newModel.get();
                    Insert(std::move(newModel));
                }
            }
            return ptr;
        }

        MaterialManager& ModelManager::GetMaterialManager()
        {
            return m_materialManager;
        }

        void ModelManager::AddModelUniformCallback(const Core::Node::Node& key, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback)
        {
            if (Model* model = GetMutable(key))
            {
                model->AddUniformCallback(shader, uniformCallback);
            }
        }
    }
}