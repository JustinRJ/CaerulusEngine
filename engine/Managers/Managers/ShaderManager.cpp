#include "stdafx.h"
#include "ShaderManager.h"

namespace Managers
{
    ShaderManager::ShaderManager(ShaderSrcManager& shaderStageManager) :
        m_ShaderStageManager(shaderStageManager)
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
        m_ShaderStageManager.Load(vertexPath, Vertex);
        m_ShaderStageManager.Load(fragmentPath, Fragment);

        auto s = std::make_shared<Shader>(
            *m_ShaderStageManager.Get(vertexPath),
            *m_ShaderStageManager.Get(fragmentPath));

        if (s->Link())
        {
            Insert(name, s);
            return true;
        }
        return false;
    }
}