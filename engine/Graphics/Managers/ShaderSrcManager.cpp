#include "stdafx.h"

#include "ShaderSrcManager.h"

namespace Graphics
{
    namespace Managers
    {
        void ShaderSrcManager::Load(const std::string& path, ShaderType type)
        {
            if (IsLoaded(path))
            {
                Core::Log::LogInDebug("\tShader stage already loaded with path: " + path);
            }
            else
            {
                Core::Log::LogMessage("\tLoading shader stage with path: " + path);
                std::shared_ptr<ShaderSrc> shaderSource = std::make_shared<ShaderSrc>(type, path);
                shaderSource->Load();

                if (shaderSource->IsCompiled())
                {
                    Insert(path, shaderSource);
                }
                else
                {
                    Core::Log::LogInDebug("Failed to compile shader source with path: " + path);
                }
            }
        }
    }
}