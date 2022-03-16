#pragma once

#include "ECS/Component.h"
#include "Pipeline/ShaderUniformCallback.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        struct Vertex;
    }
    namespace Rendering
    {
        class GLRenderer;
    }

    namespace Geometry
    {
        class Mesh;

        class CAERULUS_GRAPHICS Model : public Core::ECS::Component, public Pipeline::ShaderUniformCallback
        {
        public:
            Model(Core::ECS::Entity& entity, const std::string& path);

            bool IsLoaded() const;

            const std::vector<std::shared_ptr<Mesh>>& GetMeshes() const;

            void Update(float deltaTime) override;

        private:
            void LoadModel(std::vector<std::vector<Geometry::Vertex>>& verticesOut, std::vector<std::vector<GLuint>>& indicesOut, std::vector<std::string>& materialNamesOut);
            void CalculateTangentAndBiTangent(std::vector<std::vector<Geometry::Vertex>>& vertices);

            bool m_isLoaded;
            const std::string m_path;
            std::vector<std::shared_ptr<Mesh>> m_meshes;
        };
    }
}