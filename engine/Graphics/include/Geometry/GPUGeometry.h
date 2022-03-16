#pragma once

#include "Math/Math.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"
#include "Pipeline/ShaderUniformCallback.h"

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
                m_indexBuffer(indexData, indexCount)
            {}

            virtual ~GPUGeometry() = default;

            Pipeline::VertexArray& GetVertexArray()
            {
                return m_vertexArray;
            }

            const Pipeline::VertexArray& GetVertexArray() const
            {
                return m_vertexArray;
            }

            const Pipeline::VertexBuffer& GetVertexBuffer() const
            {
                return m_vertexBuffer;
            }

            const Pipeline::IndexBuffer& GetIndexBuffer() const
            {
                return m_indexBuffer;
            }

            const Surface::Material* GetMaterial() const
            {
                return m_material;
            }

            void SetMaterial(const Surface::Material& material)
            {
                m_material = &material;
            }

        private:
            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;


            const Surface::Material* m_material;
        };
    }
}
