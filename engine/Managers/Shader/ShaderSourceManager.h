#pragma once

#include "../Manager.h"
#include "../../Graphics/Shader/ShaderSource.h"

namespace Managers
{
    namespace Shader
    {
        using namespace Graphics::Shaders;

        class ShaderSourceManager : Manager<ShaderSource>
        {
        public:
            ShaderSourceManager();

            virtual ShaderSource* Get(const std::string& name) const;

            virtual std::vector<ShaderSource*> GetAll(const std::vector<std::string>& names) const;

            virtual bool IsLoaded(const std::string& name) const;

            virtual bool Remove(const std::string& name);

            bool Load(const std::string& name, const std::string& path, ShaderType type, const std::vector<std::string>& uniformLocations);
        };
    }
}