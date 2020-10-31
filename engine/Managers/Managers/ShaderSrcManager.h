#pragma once

#include "Managers/Manager.h"
#include "Graphics/Pipeline/ShaderSrc.h"

namespace
{
    using namespace Graphics::Pipeline;
}

namespace Managers
{
    class CAERULUS_MANAGERS ShaderSrcManager : public Manager<ShaderSrc>
    {
    public:
        ShaderSrcManager() = default;
        virtual ~ShaderSrcManager() = default;

        void Load(const std::string& path, ShaderType type);
    };
}