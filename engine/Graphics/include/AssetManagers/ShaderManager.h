#pragma once

#include "ShaderSourceManager.h"
#include "Pipeline/Shader.h"

namespace Graphics
{
    namespace AssetManagers
    {
        class CAERULUS_GRAPHICS ShaderManager : public Core::ECS::AssetManager<Pipeline::Shader>
        {
        public:
            ShaderManager(ShaderSourceManager& shaderSourceManager);

            void Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

            ShaderSourceManager& GetShaderSrcManager();

        private:
            ShaderSourceManager& m_shaderSourceManager;
        };
    }
}