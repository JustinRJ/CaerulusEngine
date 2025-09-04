#include "stdafx.h"

#include "AssetManagers/ShaderManager.h"

using namespace Core::Log;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace AssetManagers
    {
        ShaderManager::ShaderManager(ShaderSourceManager& shaderSourceManager) :
            m_shaderSourceManager(shaderSourceManager)
        {}

        void ShaderManager::Load(std::string_view name, std::string_view vertexPath, std::string_view fragmentPath)
        {
            if (Get(name.data()))
            {
                LogInDebug("Shader with name " + std::string(name) + " already loaded");
            }
            else
            {
                LogMessage("Loading shader with name " + std::string(name) + ":");
                m_shaderSourceManager.Load(vertexPath, Vertex);
                m_shaderSourceManager.Load(fragmentPath, Fragment);

                std::unique_ptr<Shader> shader = std::make_unique<Shader>(m_shaderSourceManager, vertexPath, fragmentPath);

                if (shader->IsLinked())
                {
                    Insert(name.data(), std::move(shader));
                }
                else
                {
                    LogInDebug("Failed to link shader source with name: " + std::string(name));
                }
            }
        }

        ShaderSourceManager& ShaderManager::GetShaderSrcManager()
        {
            return m_shaderSourceManager;
        }
    }
}