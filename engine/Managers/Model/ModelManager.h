#pragma once

#define CAERULUS_MANAGERS __declspec(dllexport)

#include "../Manager.h"
#include "../../Graphics/Resource/Model.h"

namespace Managers
{
    namespace Model
    {
        class ModelManager : Manager<Graphics::Resource::Model>
        {
        public:
            CAERULUS_MANAGERS ModelManager();
            CAERULUS_MANAGERS virtual ~ModelManager() {}

            CAERULUS_MANAGERS virtual Graphics::Resource::Model* Get(const std::string& name) const;

            CAERULUS_MANAGERS virtual std::vector<Graphics::Resource::Model*> GetAll(const std::vector<std::string>& names) const;

            CAERULUS_MANAGERS virtual bool IsLoaded(const std::string& name) const;

            CAERULUS_MANAGERS virtual bool Remove(const std::string& name);

            CAERULUS_MANAGERS bool Load(const std::string& name, const std::string& path);
        };
    }
}