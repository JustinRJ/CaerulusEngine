#pragma once

#include "ShaderSourceManager.h"
#include "Pipeline/Shader.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ShaderManager : public Core::ECS::AssetManager<Pipeline::Shader>
        {
        public:
            ShaderManager(ShaderSourceManager& shaderSourceManager);

            void Load(const std::string& shaderSourceName, const std::string& vertexPath, const std::string& fragmentPath);

            ShaderSourceManager& GetShaderSrcManager();

        private:
            ShaderSourceManager& m_shaderSourceManager;
        };
    }
}