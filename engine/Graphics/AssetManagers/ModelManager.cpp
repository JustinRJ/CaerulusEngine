#include "stdafx.h"

#include "AssetManagers/ModelManager.h"

#include "File/FileUtility.h"

#include "Rendering/IRenderer.h"

using namespace Core::File;
using namespace Core::Log;
using namespace Graphics::Geometry;

namespace Graphics
{
    namespace AssetManagers
    {
        ModelManager::ModelManager(MaterialManager& materialManager, Rendering::IRenderer* renderer) :
            m_materialManager(materialManager),
            m_renderer(renderer)
        {}

        void ModelManager::Load(std::string_view name, std::string_view path, std::string_view materialPath)
        {
            if (Get(name.data()))
            {
                LogInDebug("Texture " + std::string(name) + " already loaded with path: " + std::string(path));
            }
            else
            {
                LogMessage("Loading texture " + std::string(name) + " with path: " + std::string(path));
                std::unique_ptr<Model> model = std::make_unique<Model>();
                model->Load(path, m_renderer, &m_materialManager, materialPath);

                if (model->IsLoaded())
                {
                    Insert(name.data(), std::move(model));
                }
                else
                {
                    LogInDebug("Failed to load " + std::string(name) + " texture with path: " + std::string(path));
                }
            }
        }
    }
}