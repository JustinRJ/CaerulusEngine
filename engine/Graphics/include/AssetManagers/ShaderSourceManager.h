#pragma once

#include "ECS/AssetManager.h"
#include "Pipeline/ShaderSource.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ShaderSourceManager : public Core::ECS::AssetManager<Pipeline::ShaderSource>
        {
        public:
            ShaderSourceManager() = default;

            void Load(const std::string& shaderSourcePath, Pipeline::ShaderType type);
        };
    }
}