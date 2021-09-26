#pragma once

#include "MaterialManager.h"
#include "Geometry/Model.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ModelManager : public Core::Template::Manager<Geometry::Model>
        {
        public:
            ModelManager(MaterialManager& materialManager);

            void Load(Core::Node::Node* parent, const std::string& modelName, const std::string& modelPath, const std::string& materialPath = "");

            MaterialManager& GetMaterialManager();

            void AddModelUniformCallback(const std::string& modelName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformCallback);

            void SetModelLocalTransform(const std::string& modelName, const Core::Math::Transform& transform);

        private:
            MaterialManager& m_materialManager;
        };
    }
}