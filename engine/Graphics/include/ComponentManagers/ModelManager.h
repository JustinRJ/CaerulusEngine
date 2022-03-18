#pragma once

#include "Geometry/Model.h"
#include "ECS/ComponentManager.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class MaterialManager;
    }

    namespace ComponentManagers
    {
        class CAERULUS_GRAPHICS ModelManager : public Core::ECS::ComponentManager<Geometry::Model>
        {
        public:
            ModelManager(AssetManagers::MaterialManager& materialManager, Rendering::GLRenderer* renderer);

            const Geometry::Model* Load(Core::ECS::Entity& entity, const std::string& modelPath, const std::string& materialPath = "");

            AssetManagers::MaterialManager& GetMaterialManager();

            void AddModelUniformCallback(Core::ECS::Entity& key, Pipeline::Shader& shader, std::function<void(Pipeline::ShaderUniformCallback&, Pipeline::Shader& shader)> uniformCallback);

        private:
            AssetManagers::MaterialManager& m_materialManager;

            Rendering::GLRenderer* m_renderer;
        };
    }
}