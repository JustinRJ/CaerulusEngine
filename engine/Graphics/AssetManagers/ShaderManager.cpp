#include "stdafx.h"

#include "AssetManagers/ShaderManager.h"

using namespace Core::Logging;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace Managers
    {
        ShaderManager::ShaderManager(ShaderSourceManager& shaderSourceManager) :
            m_shaderSourceManager(shaderSourceManager)
        {}

        void ShaderManager::Load(const std::string& shaderSourceName, const std::string& vertexPath, const std::string& fragmentPath)
        {
            if (Get(shaderSourceName))
            {
                LogInDebug("Shader with name " + shaderSourceName + " already loaded");
            }
            else
            {
                LogMessage("Loading shader with name " + shaderSourceName + ":");
                m_shaderSourceManager.Load(vertexPath, Vertex);
                m_shaderSourceManager.Load(fragmentPath, Fragment);

                std::unique_ptr<Shader> shader = std::make_unique<Shader>(m_shaderSourceManager, vertexPath, fragmentPath);

                if (shader->IsLinked())
                {
                    Insert(shaderSourceName, std::move(shader));
                }
                else
                {
                    LogInDebug("Failed to link shader source with name: " + shaderSourceName);
                }
            }
        }

        ShaderSourceManager& ShaderManager::GetShaderSrcManager()
        {
            return m_shaderSourceManager;
        }
    }
}