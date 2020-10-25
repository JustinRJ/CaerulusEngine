#include "stdafx.h"

#include "ShaderManager.h"

namespace Managers
{
    ShaderManager::ShaderManager(ShaderSrcManager& shaderStageManager) :
        m_shaderStageManager(shaderStageManager)
    {}

    bool ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Shader with name " + name + " already loaded");
            return false;
        }

        Log::LogMessage("Loading shader with name " + name + ":");
        m_shaderStageManager.Load(vertexPath, Vertex);
        m_shaderStageManager.Load(fragmentPath, Fragment);

        auto s = std::make_shared<Shader>(
            m_shaderStageManager.Get(vertexPath),
            m_shaderStageManager.Get(fragmentPath));

        if (s->IsLinked())
        {
            Insert(name, s);
            return true;
        }
        return false;
    }
}