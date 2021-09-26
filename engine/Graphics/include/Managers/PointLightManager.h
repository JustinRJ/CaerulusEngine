#pragma once

#include "Template/Manager.h"
#include "Lighting/PointLight.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS PointLightManager : public Core::Template::Manager<Lighting::PointLight>
        {
        public:
            PointLightManager(ShaderManager& shaderManager);

            void Create(Core::Node::Node* parent, const std::string& lightName, const Core::Math::vec3& position, const Core::Math::vec3& colour);

            void AddPointLightUniformCallback(const std::string& lightName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformCallback);

        private:
            ShaderManager& m_shaderManager;
        };
    }
}