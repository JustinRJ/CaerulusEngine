#include "stdafx.h"
#include "ShaderManager.h"

namespace Managers
{
    ShaderManager::ShaderManager(ShaderSourceManager& shaderStageManager) :
        m_ShaderStageManager(shaderStageManager)
    {}

    bool ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath,
        const std::vector<std::string>& vertexUniformLocations, const std::vector<std::string>& fragmentUniformLocations)
    {
        using namespace Core::Logging;
        if (IsLoaded(name))
        {
            Log::LogInDebug("Shader with name " + name + " already loaded with paths: \n\t" + vertexPath + "\n\t" + fragmentPath);
            return false;
        }
        Log::LogMessage("Loading shader " + name + " with paths: \n\t" + vertexPath + "\n\t" + fragmentPath);
        m_ShaderStageManager.Load(vertexPath, vertexPath, Vertex, vertexUniformLocations);
        m_ShaderStageManager.Load(fragmentPath, fragmentPath, Fragment, fragmentUniformLocations);

        auto s = std::make_shared<Shader>();
        s->Compile(*m_ShaderStageManager.Get(vertexPath), *m_ShaderStageManager.Get(fragmentPath));
        Insert(name, s);
        return true;
    }
}