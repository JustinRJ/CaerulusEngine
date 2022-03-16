#pragma once

#include "ECS/ComponentManager.h"
#include "Lighting/PointLight.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS PointLightManager : public Core::ECS::ComponentManager<Lighting::PointLight>
        {
        public:
            PointLightManager() = default;

            const Lighting::PointLight* Create(Core::ECS::Entity& entity, const Core::Math::vec3& colour);

            void AddPointLightUniformCallback(Core::ECS::Entity& entity, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback);
        };
    }
}