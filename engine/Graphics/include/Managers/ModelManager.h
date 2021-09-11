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

            void Load(const std::string& modelName, const std::string& modelPath, const std::string& materialPath = "");

            MaterialManager& GetMaterialManager();

            void AddModelUniformFunctor(const std::string& modelName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformFunctor);

            void SetModelTransform(const std::string& modelName, const Core::Math::mat4& transform);

        private:
            MaterialManager& m_materialManager;
        };
    }
}