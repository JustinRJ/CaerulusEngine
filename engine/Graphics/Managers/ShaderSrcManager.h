#pragma once

#include "Core/Template/Manager.h"
#include "Graphics/Pipeline/ShaderSrc.h"

namespace
{
    using namespace Graphics::Pipeline;
}

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_CORE ShaderSrcManager : public Core::Template::Manager<ShaderSrc>
        {
        public:
            ShaderSrcManager() = default;
            ~ShaderSrcManager() = default;

            void Load(const std::string& path, ShaderType type);
        };
    }
}