#pragma once

#include "../Manager.h"
#include "../../Graphics/PipeLine/ShaderSource.h"

namespace Managers
{
    namespace PipeLine
    {
        using namespace Graphics::PipeLine;

        class ShaderSourceManager : Manager<ShaderSource>
        {
        public:
            ShaderSourceManager();
            virtual ~ShaderSourceManager() {}

            virtual ShaderSource* Get(const std::string& name) const;

            virtual std::vector<ShaderSource*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& name, const std::string& path, ShaderType type, const std::vector<std::string>& uniformLocations);
        };
    }
}