#pragma once

#include "Math/Math.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"
#include "Pipeline/ShaderUniformCallback.h"
#include "Rendering/GLRenderer.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        class GPUGeometry : public Pipeline::ShaderUniformCallback
        {
        public:
            GPUGeometry(const void* vertData, size_t vertSize, const unsigned int* indexData, size_t indexCount) :
                m_vertexBuffer(vertData, vertSize),
                m_indexBuffer(indexData, indexCount),
                m_material(nullptr),
                m_renderer(nullptr)
            {}

            virtual ~GPUGeometry() = default;

            Pipeline::VertexArray& GetVertexArray()
            {
                return m_vertexArray;
            }

            Pipeline::VertexBuffer& GetVertexBuffer()
            {
                return m_vertexBuffer;
            }

            Pipeline::IndexBuffer& GetIndexBuffer()
            {
                return m_indexBuffer;
            }

            Surface::Material* GetMaterial()
            {
                return m_material;
            }

            void SetMaterial(Surface::Material* material)
            {
                m_material = material;
            }

            Rendering::IRenderer* GetRenderer()
            {
                return m_renderer;
            }

            void SetRenderer(Rendering::IRenderer* renderer)
            {
                m_renderer = renderer;
            }

            void Draw()
            {
                if (m_renderer)
                {
                    m_renderer->Draw(*this);
                }
            }

        private:
            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;
            Surface::Material* m_material;

            // TODO - make this static so every piece of geometry uses the same render instance, don't have to set it in Model
            Rendering::IRenderer* m_renderer;
        };
    }
}
