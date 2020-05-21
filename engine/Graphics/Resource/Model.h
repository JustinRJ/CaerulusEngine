#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "../Geometry/Mesh.h"
#include "../Resource/Material.h"
#include "../../Core/Math/Math.h"
#include "../../Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Resource
    {
        using namespace Geometry;
        using namespace Core::Math;

        class CAERULUS_GRAPHICS Model : public Core::Interface::NonCopyable
        {
        public:
            Model(const std::string& path);
            virtual ~Model() = default;

            bool IsLoaded() const;

            void Draw(bool wireframe, std::shared_ptr<Material> defaultMaterial) const;
            void DrawMesh(bool wireframe, unsigned int mesh) const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

            const std::vector<std::shared_ptr<Material>>& GetMaterials() const;
            void SetMaterials(std::vector<std::shared_ptr<Material>> materials);

        private:

            bool m_isLoaded;
            std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
            std::vector<std::shared_ptr<Material>> m_materials;
        };
    }
}

namespace std
{
    template<>
    struct hash<Graphics::Resource::Vertex>
    {
        size_t operator()(const Graphics::Resource::Vertex& vertex) const
        {
            using namespace Core::Math;
            return
                ((hash<vec3>()(vertex.Position) ^
                (hash<vec3>()(vertex.Normal) << 1)) >> 1) ^
                    (hash<vec2>()(vertex.TexCoords) << 1);
        }
    };
}