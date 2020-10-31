#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "ShaderSrcManager.h"
#include "Graphics/Pipeline/Shader.h"

namespace Managers
{
    using namespace Graphics::Pipeline;
    class CAERULUS_MANAGERS ShaderManager : public Manager<Shader>
    {
    public:
        ShaderManager(ShaderSrcManager& shaderStageManager);
        virtual ~ShaderManager() = default;

        void Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    private:
        ShaderSrcManager& m_shaderStageManager;
    };
}