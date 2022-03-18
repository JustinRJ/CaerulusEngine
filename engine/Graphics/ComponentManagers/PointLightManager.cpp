#include "stdafx.h"

#include "ComponentManagers/PointLightManager.h"

using namespace Graphics::Lighting;

namespace Graphics
{
    namespace ComponentManagers
    {
        const PointLight* PointLightManager::Create(Core::ECS::Entity& entity, const Core::Math::vec3& colour)
        {
            std::unique_ptr<Lighting::PointLight> pointLight = std::make_unique<Lighting::PointLight>(entity);
            PointLight* ptr = pointLight.get();
            pointLight->SetColour(colour);
            Insert(&entity, std::move(pointLight));
            return ptr;
        }

        void PointLightManager::AddPointLightUniformCallback(Core::ECS::Entity& entity, Pipeline::Shader& shader, std::function<void(Pipeline::ShaderUniformCallback&, Pipeline::Shader& shader)> uniformCallback)
        {
            if (PointLight* light = GetMutable(&entity))
            {
                light->AddUniformCallback(shader, uniformCallback);
            }
        }
    }
}