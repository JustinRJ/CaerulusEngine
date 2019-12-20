#pragma once

#include "../Manager.h"
#include "../../Graphics/PipeLine/ShaderSource.h"

namespace Managers
{
    using namespace Graphics::PipeLine;
    class ShaderSourceManager : public Manager<ShaderSource>
    {
    public:
        ShaderSourceManager();
        virtual ~ShaderSourceManager() = default;

        bool Load(const std::string& name, const std::string& path, ShaderType type, const std::vector<std::string>& uniformLocations);
    };
}