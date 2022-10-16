#pragma once

#include "ECS/AssetManager.h"
#include "Pipeline/ShaderSource.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS ShaderSourceManager : public Core::ECS::AssetManager<Pipeline::ShaderSource>
        {
        public:
            ShaderSourceManager() = default;

            void Load(std::string_view path, Pipeline::ShaderType type);
        };
    }
}