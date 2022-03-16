#pragma once

#include "Geometry/Model.h"
#include "Template/ComponentManager.h"
#include "ComponentManagers/PointLightManager.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ModelManager : public Core::Template::ComponentManager<Geometry::Model>
        {
        public:
            ModelManager(MaterialManager& materialManager);

            const Geometry::Model* Load(Core::Node::Node& node, const std::string& modelPath, const std::string& materialPath = "");

            MaterialManager& GetMaterialManager();

            void AddModelUniformCallback(const Core::Node::Node& key, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback);

        private:
            MaterialManager& m_materialManager;
        };
    }
}