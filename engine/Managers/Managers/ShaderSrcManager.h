#pragma once

#include "../Manager.h"
#include "../../Graphics/PipeLine/ShaderSrc.h"

namespace Managers
{
    using namespace Graphics::PipeLine;
    class CAERULUS_MANAGERS ShaderSrcManager : public Manager<ShaderSrc>
    {
    public:
        ShaderSrcManager();
        virtual ~ShaderSrcManager() = default;

        bool Load(const std::string& path, ShaderType type);
    };
}