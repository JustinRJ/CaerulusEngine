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
        };
    }
}