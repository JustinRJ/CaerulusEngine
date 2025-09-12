#pragma once

#include "Pipeline/ShaderUniformCallback.h"

#include "Geometry/Model.h"
#include "Surface/Material.h"

namespace Graphics
{
    class RenderInstance : public Pipeline::ShaderUniformCallback
    {
    public:
        Geometry::Model* Model = nullptr;
        // TODO - when this is set, override the materials used within the Models
        // material this easily allows variations of models without wasting resources
        std::vector<Surface::Material*> MeshOverrideMaterial;

        void Tick(float deltaTime)
        {
            if (Model)
            {
                InvokeUniformCallbacks();
                for (const std::unique_ptr<Geometry::Mesh>& mesh : Model->GetMeshes())
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