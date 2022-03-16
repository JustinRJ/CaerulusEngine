#pragma once

#include "Geometry/Model.h"
#include "ECS/ComponentManager.h"

namespace Graphics
{
    namespace Managers
    {
        class MaterialManager;

        class CAERULUS_GRAPHICS ModelManager : public Core::ECS::ComponentManager<Geometry::Model>
        {
        public:
            ModelManager(MaterialManager& materialManager, const Rendering::GLRenderer* renderer);

            const Geometry::Model* Load(Core::ECS::Entity& entity, const std::string& modelPath, const std::string& materialPath = "");

            MaterialManager& GetMaterialManager();

            void AddModelUniformCallback(Core::ECS::Entity& key, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback);

        private:
            MaterialManager& m_materialManager;

            const Rendering::GLRenderer* m_renderer;
        };
    }
}