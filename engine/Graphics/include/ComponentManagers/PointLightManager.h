#pragma once

#include "ECS/ComponentManager.h"
#include "Lighting/PointLight.h"

namespace Graphics
{
    namespace ComponentManagers
    {
        class CAERULUS_GRAPHICS PointLightManager : public Core::ECS::ComponentManager<Lighting::PointLight>
        {
        public:
            PointLightManager() = default;

            const Lighting::PointLight* Create(Core::ECS::Entity& entity, const Core::Math::vec3& colour);

            void AddPointLightUniformCallback(Core::ECS::Entity& entity, Pipeline::Shader& shader, std::function<void(Pipeline::ShaderUniformCallback&, Pipeline::Shader& shader)> uniformCallback);
        };
    }
}