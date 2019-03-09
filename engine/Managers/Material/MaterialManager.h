//#pragma once
//
//#include "../Manager.h"
//#include "../Texture/TextureManager.h"
//#include "../../Graphics/Resource/Material.h"
//
//namespace Managers
//{
//    namespace Material
//    {
//        class MaterialManager : Manager<Graphics::Resource::Material>
//        {
//        private:
//            Texture::TextureManager& m_TextureManager;
//
//        public:
//            MaterialManager(Texture::TextureManager& textureManager);
//
//            virtual Graphics::Resource::Material* Get(const std::string& name) const;
//
//            virtual std::vector<Graphics::Resource::Material*> GetAll(const std::vector<std::string>& names) const;
//
//            virtual bool IsLoaded(const std::string& name) const;
//
//            virtual bool Remove(const std::string& name);
//
//            //key value is material name defined in .mtl file
//            bool Load(const std::string& path);
//
//            bool Create(const std::string& name, const std::vector<Graphics::Resource::Texture*>& textures);
//
//            bool SetTexture(Graphics::Resource::Texture* texture, Graphics::Resource::MaterialType materialType, const std::string& name);
//        };
//    }
//}