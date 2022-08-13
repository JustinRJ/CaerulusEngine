#include "stdafx.h"

#include "AssetManagers/ShaderManager.h"

using namespace Core::Logging;
using namespace Graphics::Pipeline;

namespace Graphics
{
    namespace AssetManagers
    {
        ShaderManager::ShaderManager(ShaderSourceManager& shaderSourceManager) :
            m_shaderSourceManager(shaderSourceManager)
        {}

        void ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
        {
            if (Get(name))
            {
                LogInDebug("Shader with name " + name + " already loaded");
            }
            else
            {
                LogMessage("Loading shader with name " + name + ":");
                m_shaderSourceManager.Load(vertexPath, Vertex);
                m_shaderSourceManager.Load(fragmentPath, Fragment);

                std::unique_ptr<Shader> shader = std::make_unique<Shader>(m_shaderSourceManager, vertexPath, fragmentPath);

                if (shader->IsLinked())
                {
                    Insert(name, std::move(shader));
                }
                else
                {
                    LogInDebug("Failed to link shader source with name: " + name);
                }
            }
        }

        ShaderSourceManager& ShaderManager::GetShaderSrcManager()
        {
            return m_shaderSourceManager;
        }
    }
}