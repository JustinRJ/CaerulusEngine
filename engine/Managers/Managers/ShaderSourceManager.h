#pragma once

#include "../Manager.h"
#include "../../Graphics/PipeLine/ShaderSource.h"

namespace Managers
{
    using namespace Graphics::PipeLine;
    class CAERULUS_MANAGERS ShaderSourceManager : public Manager<ShaderSource>
    {
    public:
        ShaderSourceManager();
        virtual ~ShaderSourceManager() = default;

        bool Load(const std::string& path, ShaderType type);
    };
}