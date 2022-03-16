#pragma once

#include "Template/ComponentManager.h"
#include "Lighting/PointLight.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS PointLightManager : public Core::Template::ComponentManager<Lighting::PointLight>
        {
        public:
            PointLightManager() = default;

            const Lighting::PointLight* Create(Core::Node::Node& node, const Core::Math::vec3& colour);

            void AddPointLightUniformCallback(const Core::Node::Node& node, const Pipeline::Shader& shader, std::function<void(const Pipeline::ShaderUniformCallback&, const Pipeline::Shader& shader)> uniformCallback);
        };
    }
}