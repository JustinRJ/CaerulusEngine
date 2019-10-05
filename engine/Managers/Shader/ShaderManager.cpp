#include "stdafx.h"
#include "ShaderManager.h"

namespace Managers
{
    namespace PipeLine
    {

        ShaderManager::ShaderManager(ShaderSourceManager& shaderStageManager) :
            m_ShaderStageManager(shaderStageManager)
        {}

        bool ShaderManager::Load(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath,
            const std::vector<std::string>& vertexUniformLocations, const std::vector<std::string>& fragmentUniformLocations)
        {
            if (IsLoaded(name))
            {
                return false;
            }
            
            m_ShaderStageManager.Load(vertexPath, vertexPath, Vertex, vertexUniformLocations);
            m_ShaderStageManager.Load(fragmentPath, fragmentPath, Fragment, fragmentUniformLocations);

            Shader* s = new Shader();
            s->Compile(*m_ShaderStageManager.Get(vertexPath), *m_ShaderStageManager.Get(fragmentPath));
            Manager::Insert(name, s);
            return true;
        }

        Shader* ShaderManager::Get(const std::string& name) const
        {
            return Manager::Get(name);
        }

        std::vector<Shader*> ShaderManager::GetAll(const std::vector<std::string>& names) const
        {
            return Manager::GetAll(names);
        }

        bool ShaderManager::IsLoaded(const std::string& name) const
        {
            return Manager::IsLoaded(name);
        }

        bool ShaderManager::Remove(const std::string& name)
        {
            return Manager::Remove(name);
        }
    }
}