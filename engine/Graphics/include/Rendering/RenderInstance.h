#pragma once

#include "ECS/Component.h"
#include "Pipeline/ShaderUniformCallback.h"

#include "Geometry/Model.h"
#include "Surface/Material.h"

namespace Graphics
{
    class RenderInstance : public Core::ECS::Component, public Pipeline::ShaderUniformCallback
    {
    public:
        std::shared_ptr<Geometry::Model> Model = nullptr;
        std::vector<std::shared_ptr<Surface::Material>> MeshOverrideMaterial;

        RenderInstance(Core::ECS::Entity& e) :
            Core::ECS::Component(e)
        {}

        void Update(float deltaTime) override
        {
            if (Model)
            {
                InvokeUniformCallbacks();
                for (const std::shared_ptr<Geometry::Mesh>& mesh : Model->GetMeshes())
                {
                    if (mesh && mesh->GetRenderer())
                    {
                        mesh->InvokeUniformCallbacks();
                        if (auto fileMaterial = mesh->GetFileMaterial())
                        {
                            fileMaterial->Bind();
                            fileMaterial->InvokeUniformCallbacks();
                            mesh->Draw();
                        }
                    }
                }
            }
        }
    };
}