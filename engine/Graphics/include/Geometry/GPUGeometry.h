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
            GPUGeometry(const void* vertData, size_t vertSize, const uint32_t* indexData, size_t indexCount) :
                m_vertexBuffer(vertData, vertSize),
                m_indexBuffer(indexData, indexCount),
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
            Rendering::IRenderer* m_renderer;
        };
    }
}
