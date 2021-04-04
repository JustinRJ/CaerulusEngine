#pragma once


#include "ShaderSrcManager.h"
#include "Graphics/Pipeline/Shader.h"

namespace
{
    using namespace Graphics::Pipeline;
}

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ShaderManager : public Core::Template::Manager<Shader>
        {
        public:
            ShaderManager(ShaderSrcManager& shaderStageManager);
            ~ShaderManager() = default;

            void Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

        private:
            ShaderSrcManager& m_shaderStageManager;
        };
    }
}