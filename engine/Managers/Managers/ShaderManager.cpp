#include "stdafx.h"

#include "ShaderManager.h"

namespace Managers
{
    ShaderManager::ShaderManager(ShaderSrcManager& shaderStageManager) :
        m_shaderStageManager(shaderStageManager)
    {}

    void ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    {
        if (IsLoaded(name))
        {
            Log::LogInDebug("Shader with name " + name + " already loaded");
            return;
        }

        Log::LogMessage("Loading shader with name " + name + ":");
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
            Log::LogInDebug("Failed to link shader source with name: " + name);
        }
    }
}