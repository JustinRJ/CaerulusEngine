#include "stdafx.h"

#include "ModelManager.h"
#include <iostream>

namespace Managers
{
    namespace Model
    {
        ModelManager::ModelManager()
        {
        }

        bool ModelManager::Load(const std::string& name, const std::string& path)
        {
            if (IsLoaded(name))
            {
                return false;
            }
            std::cout << "Loading model " + name + " with path " + path << std::endl;
            Manager::Insert(name, new Graphics::Resource::Model(path));
            return true;
        }

        Graphics::Resource::Model* ModelManager::Get(const std::string& name) const
        {
            return Manager::Get(name);
        }

        std::vector<Graphics::Resource::Model*> ModelManager::GetAll(const std::vector<std::string>& names) const
        {
            return Manager::GetAll(names);
        }

        bool ModelManager::IsLoaded(const std::string& name) const
        {
            return Manager::IsLoaded(name);
        }

        bool ModelManager::Remove(const std::string& name)
        {
            return Manager::Remove(name);
        }
    }
}