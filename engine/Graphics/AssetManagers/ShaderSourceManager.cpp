#include "stdafx.h"

#include "AssetManagers/ShaderSourceManager.h"

using namespace Core::Logging;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace Managers
    {
        void ShaderSourceManager::Load(const std::string& shaderSourcePath, ShaderType type)
        {
            if (Get(shaderSourcePath))
            {
                LogInDebug("\tShader stage already loaded with path: " + shaderSourcePath);
            }
            else
            {
                LogMessage("\tLoading shader stage with path: " + shaderSourcePath);
                std::unique_ptr<ShaderSource> shaderSource = std::make_unique<ShaderSource>(type, shaderSourcePath);
                shaderSource->Load();

                if (shaderSource->IsCompiled())
                {
                    Insert(shaderSourcePath, std::move(shaderSource));
                }
                else
                {
                    LogInDebug("Failed to compile shader source with path: " + shaderSourcePath);
                }
            }
        }
    }
}