#pragma once

#include "../Manager.h"
#include "../../Graphics/Resource/Model.h"

namespace Managers
{
    namespace Model
    {
        class ModelManager : Manager<Graphics::Resource::Model>
        {
        public:
            ModelManager();

            virtual Graphics::Resource::Model* Get(const std::string& name) const;

            virtual std::vector<Graphics::Resource::Model*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& name, const std::string& path);
        };
    }
}