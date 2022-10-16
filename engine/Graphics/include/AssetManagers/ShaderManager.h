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

            void Load(std::string_view name, std::string_view vertexPath, std::string_view fragmentPath);

            ShaderSourceManager& GetShaderSrcManager();

        private:
            ShaderSourceManager& m_shaderSourceManager;
        };
    }
}