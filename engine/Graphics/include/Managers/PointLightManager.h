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

            void Create(const std::string& lightName, const Core::Math::vec3& position, const Core::Math::vec3& colour);

            void AddPointLightUniformFunctor(const std::string& lightName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformFunctor);

        private:
            ShaderManager& m_shaderManager;
        };
    }
}