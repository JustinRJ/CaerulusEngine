#include "stdafx.h"

#include "AssetManagers/ShaderSourceManager.h"

using namespace Core::Logging;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace AssetManagers
    {
        void ShaderSourceManager::Load(const std::string& path, ShaderType type)
        {
            if (Get(path))
            {
                LogInDebug("\tShader stage already loaded with path: " + path);
            }
            else
            {
                LogMessage("\tLoading shader stage with path: " + path);
                std::shared_ptr<ShaderSource> shaderSource = std::make_unique<ShaderSource>(type, path);
                shaderSource->Load();

                if (shaderSource->IsCompiled())
                {
                    Insert(path, shaderSource);
                }
                else
                {
                    LogInDebug("Failed to compile shader source with path: " + path);
                }
            }
        }
    }
}