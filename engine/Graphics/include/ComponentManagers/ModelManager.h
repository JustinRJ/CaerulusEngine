#pragma once

#include "Geometry/Model.h"
#include "ECS/ComponentManager.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class MaterialManager;
    }

    namespace ComponentManagers
    {
        // TODO - use ModelInstance component class which has raw ptrs to meshes and materials so that can have different combos of mesh/material
        //      - remove all component behaviour from current Model class and move to ModelInstance, Model should be stored within an AssetManager
        class CAERULUS_GRAPHICS ModelManager : public Core::ECS::ComponentManager<Geometry::Model>
        {
        public:
            ModelManager() = default;
        };
    }
}