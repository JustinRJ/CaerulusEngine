#include "stdafx.h"

#include "ShaderSrcManager.h"

using namespace Core::Logging;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace Managers
    {
        void ShaderSrcManager::Load(const std::string& path, ShaderType type)
        {
            if (IsLoaded(path))
            {
                LogInDebug("\tShader stage already loaded with path: " + path);
            }
            else
            {
                LogMessage("\tLoading shader stage with path: " + path);
                std::shared_ptr<ShaderSrc> shaderSource = std::make_shared<ShaderSrc>(type, path);
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