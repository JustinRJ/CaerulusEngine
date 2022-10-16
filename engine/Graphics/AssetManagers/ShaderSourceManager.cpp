#include "stdafx.h"

#include "AssetManagers/ShaderSourceManager.h"

using namespace Core::Log;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace AssetManagers
    {
        void ShaderSourceManager::Load(std::string_view path, ShaderType type)
        {
            if (Get(path.data()))
            {
                LogInDebug("\tShader stage already loaded with path: " + std::string(path));
            }
            else
            {
                LogMessage("\tLoading shader stage with path: " + std::string(path));
                std::shared_ptr<ShaderSource> shaderSource = std::make_unique<ShaderSource>(type, path);
                shaderSource->Load();

                if (shaderSource->IsCompiled())
                {
                    Insert(path.data(), shaderSource);
                }
                else
                {
                    LogInDebug("Failed to compile shader source with path: " + std::string(path));
                }
            }
        }
    }
}