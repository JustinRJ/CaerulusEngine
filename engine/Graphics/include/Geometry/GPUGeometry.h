#pragma once

#include "Node/Node.h"
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
        class GPUGeometry : public Core::Node::Node, public Pipeline::ShaderUniformCallback
        {
        public:
            GPUGeometry(
                Core::Node::Node* parent,
                const Managers::ShaderManager& shaderManager,
                const void* vertData, size_t vertSize, const unsigned int* indexData, size_t indexCount) :
                Node::Node(parent),
                Pipeline::ShaderUniformCallback(shaderManager),
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

            const std::string& GetMaterialName() const
            {
                return m_materialName;
            }

            void SetMaterialName(const std::string& materialName)
            {
                m_materialName = materialName;
            }

        private:
            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;

            std::string m_materialName;
        };
    }
}
