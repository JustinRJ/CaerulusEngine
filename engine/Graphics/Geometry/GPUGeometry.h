#pragma once

#include "Core/Math/Math.h"
#include "Core/Node/Node.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"
#include "Graphics/Pipeline/ShaderUniformFunctor.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        class GPUGeometry : public Core::Node::Node, public Pipeline::ShaderUniformFunctor
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

            std::shared_ptr<Surface::Material> GetMaterial() const
            {
                return m_material;
            }

            void SetMaterial(std::shared_ptr<Surface::Material> material)
            {
                m_material = material;
            }

        private:
            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;

            std::shared_ptr<Surface::Material> m_material;
        };
    }
}
