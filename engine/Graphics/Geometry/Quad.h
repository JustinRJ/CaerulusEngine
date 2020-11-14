#pragma once

#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace Graphics
{
    namespace Surface
    {
        class Material;
    }

    namespace Geometry
    {
        const static GLfloat s_QuadVertices[] =
        {
            -0.5, -0.5, // bottom left
            0.5, -0.5,  // bottom right
            0.5, 0.5,   // top right
            -0.5, 0.5   // top left
        };

        const static GLuint s_QuadIndices[] =
        {
            0, 1, 2,    0, 3, 2
        };

        class Quad
        {
        public:
            Quad() :
                m_vertexBuffer(s_QuadVertices, 4 * 2 * sizeof(GLfloat)),
                m_indexBuffer(s_QuadIndices, 6)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(2);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Quad() = default;

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
