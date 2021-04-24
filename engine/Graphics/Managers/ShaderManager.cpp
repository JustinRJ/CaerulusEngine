#include "stdafx.h"

#include "ShaderManager.h"

namespace
{
    using namespace Core::Logging;
    using namespace Graphics::Pipeline;
}

namespace Graphics
{
    namespace Managers
    {
        ShaderManager::ShaderManager(ShaderSrcManager& shaderStageManager) :
            m_shaderStageManager(shaderStageManager)
        {}

        void ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
        {
            if (IsLoaded(name))
            {
                LogInDebug("Shader with name " + name + " already loaded");
            }
            else
            {
                LogMessage("Loading shader with name " + name + ":");
                m_shaderStageManager.Load(vertexPath, Vertex);
                m_shaderStageManager.Load(fragmentPath, Fragment);

                std::shared_ptr<Shader> shader = std::make_shared<Shader>(
                    m_shaderStageManager.Get(vertexPath),
                    m_shaderStageManager.Get(fragmentPath));

                if (shader->IsLinked())
                {
                    Insert(name, shader);
                }
                else
                {
                    LogInDebug("Failed to link shader source with name: " + name);
                }
            }
        }
    }
}