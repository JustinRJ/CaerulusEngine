#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "../Geometry/Mesh.h"
#include "../Resource/Material.h"
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace Resource
    {
        using namespace Geometry;
        using namespace Core::Math;

        class CAERULUS_GRAPHICS Model
        {
        public:
            Model(const std::string& path);
            ~Model();

            bool IsLoaded() const;

            void Draw(bool wireframe, std::shared_ptr<Material> defaultMaterial) const;
            void DrawMesh(bool wireframe, unsigned int mesh) const;

            const std::vector<Mesh>& GetMeshes() const;

            const std::vector<std::shared_ptr<Material>>& GetMaterials() const;
            void SetMaterials(std::vector<std::shared_ptr<Material>> materials);

        private:

            bool m_IsLoaded;
            std::string m_Path;
            std::vector<Mesh>& m_Meshes;
            std::vector<std::shared_ptr<Material>> m_Materials;
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