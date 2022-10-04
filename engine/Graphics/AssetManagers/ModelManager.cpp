#include "stdafx.h"

#include "AssetManagers/ModelManager.h"

#include "File/FileUtility.h"

#include "Rendering/IRenderer.h"

using namespace Core::File;
using namespace Core::Logging;
using namespace Graphics::Geometry;

namespace Graphics
{
    namespace AssetManagers
    {
        ModelManager::ModelManager(MaterialManager& materialManager, Rendering::IRenderer* renderer) :
            m_materialManager(materialManager),
            m_renderer(renderer)
        {}

        void ModelManager::Load(const std::string& name, const std::string& path, const std::string& materialPath)
        {
            if (Get(name))
            {
                LogInDebug("Texture " + name + " already loaded with path: " + path);
            }
            else
            {
                LogMessage("Loading texture " + name + " with path: " + path);
                std::shared_ptr<Model> model = std::make_unique<Model>();
                model->Load(path, m_renderer, &m_materialManager, materialPath);

                if (model->IsLoaded())
                {
                    Insert(name, model);
                }
                else
                {
                    LogInDebug("Failed to load " + name + " texture with path: " + path);
                }
            }
        }
    }
}