#pragma once

#include "Core/Template/Manager.h"
#include "Graphics/Pipeline/ShaderSrc.h"

namespace Graphics
{
    namespace Managers
    {
        class CAERULUS_GRAPHICS ShaderSrcManager : public Core::Template::Manager<Pipeline::ShaderSrc>
        {
        public:
            ShaderSrcManager() = default;
            ~ShaderSrcManager() = default;

            void Load(const std::string& path, Pipeline::ShaderType type);
        };
    }
}