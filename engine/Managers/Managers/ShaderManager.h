#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "ShaderSourceManager.h"
#include "../../Graphics/PipeLine/Shader.h"

namespace Managers
{
    using namespace Graphics::PipeLine;
    class CAERULUS_MANAGERS ShaderManager : public Manager<Shader>
    {
    public:
        ShaderManager(ShaderSourceManager& shaderStageManager);
        virtual ~ShaderManager() = default;

        bool Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);

    private:
        ShaderSourceManager& m_ShaderStageManager;
    };
}