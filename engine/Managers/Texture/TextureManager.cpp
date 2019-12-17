#include "stdafx.h"
#include "TextureManager.h"

namespace Managers
{
    namespace Texture
    {
        TextureManager::TextureManager()
        {
        }

        bool TextureManager::Load(const std::string& name, const std::string& path)
        {
            if (IsLoaded(name))
            {
                std::cerr << "Texture " + name + " already loaded with path \t\t" + path << std::endl;
                return false;
            }
            std::cout << "Loading texture " + name + " with path \t\t" + path << std::endl;
            Graphics::Resource::Texture *t = new Graphics::Resource::Texture();
            t->Load(path.c_str(), true);
            Insert(name, t);
            return true;
        }

        bool TextureManager::LoadHDR(const std::string& name, const std::string& path)
        {
            if (IsLoaded(name))
            {
                std::cerr << "Texture " + name + " already loaded with path \t\t" + path << std::endl;
                return false;
            }
            std::cout << "Loading texture " + name + " with path \t\t" + path << std::endl;
            Graphics::Resource::Texture *t = new Graphics::Resource::Texture();
            t->LoadHDR(path.c_str(), true);
            Insert(name, t);
            return true;
        }
    }
}