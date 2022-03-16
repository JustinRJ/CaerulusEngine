#include "stdafx.h"

#include "ComponentManagers/PointLightManager.h"

using namespace Graphics::Lighting;

namespace Graphics
{
    namespace Managers
    {
        const PointLight* PointLightManager::Create(Core::Node::Node& node, const Core::Math::vec3& colour)
        {
            std::unique_ptr<Lighting::PointLight> pointLight = std::make_unique<Lighting::PointLight>(node);
            PointLight* ptr = pointLight.get();
            pointLight->SetColour(colour);
            Insert(std::move(pointLight));
            return ptr;
        }

        void PointLightManager::AddPointLightUniformCallback(const Core::Node::Node& node, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback)
        {
            if (PointLight* light = GetMutable(node))
            {
                light->AddUniformCallback(shader, uniformCallback);
            }
        }
    }
}