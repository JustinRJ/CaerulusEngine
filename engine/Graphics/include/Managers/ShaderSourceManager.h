#pragma once

#include "Template/Manager.h"
#include "Pipeline/ShaderSource.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ShaderSourceManager : public Core::Template::Manager<Pipeline::ShaderSource>
        {
        public:
            ShaderSourceManager() = default;

            void Load(const std::string& shaderSourcePath, Pipeline::ShaderType type);
        };
    }
}