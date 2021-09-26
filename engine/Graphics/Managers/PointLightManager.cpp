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

        void PointLightManager::Create(Core::Node::Node* parent, const std::string& lightName, const Core::Math::vec3& position, const Core::Math::vec3& colour)
        {
            std::unique_ptr<Lighting::PointLight> pointLight = std::make_unique<Lighting::PointLight>(parent, m_shaderManager);
            pointLight->SetPosition(position);
            pointLight->SetColour(colour);
            Insert(lightName, std::move(pointLight));
        }

        void PointLightManager::AddPointLightUniformCallback(const std::string& lightName, const std::string& shaderName, std::function<void(const Pipeline::Shader& shader)> uniformCallback)
        {
            if (PointLight* pointLight = GetMutable(lightName))
            {
                pointLight->AddUniformCallback(shaderName, uniformCallback);
            }
        }
    }
}