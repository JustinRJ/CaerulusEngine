#include "stdafx.h"

#include "Managers/PointLightManager.h"

using namespace Graphics::Lighting;

namespace Graphics
{
    namespace Managers
    {
        PointLightManager::PointLightManager(ShaderManager& shaderManager) :
            m_shaderManager(shaderManager)
        {}

        void PointLightManager::Create(const std::string& lightName, const Core::Math::vec3& position, const Core::Math::vec3& colour)
        {
            std::unique_ptr<Lighting::PointLight> pointLight = std::make_unique<Lighting::PointLight>(m_shaderManager);
            pointLight->SetPosition(position);
            pointLight->SetColour(colour);
            Insert(lightName, std::move(pointLight));
        }

        void PointLightManager::AddPointLightUniformFunctor(const std::string& lightName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformFunctor)
        {
            if (PointLight* pointLight = GetMutable(lightName))
            {
                pointLight->AddUniformFunctor(shaderName, uniformFunctor);
            }
        }

    }
}