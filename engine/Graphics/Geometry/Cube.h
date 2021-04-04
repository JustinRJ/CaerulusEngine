#pragma once

#include "Core/Node/Node.h"
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
        const static GLfloat s_CubeVertices[] =
        {
            -0.5, -0.5, 0.5,  // front bottom left
             0.5, -0.5, 0.5,  // front bottom right
             0.5,  0.5, 0.5,  // front top right
            -0.5,  0.5, 0.5,  // front top left
            -0.5, -0.5, -0.5, // back bottom left
             0.5, -0.5, -0.5, // back bottom right
             0.5,  0.5, -0.5, // back top right
            -0.5,  0.5, -0.5, // back top left
        };

        const static GLuint s_CubeIndices[] =
        {
            0, 3, 2, 0, 1, 2, // front
            4, 0, 1, 4, 5, 1, // left
            3, 7, 6, 3, 2, 6, // right
            1, 2, 6, 1, 5, 6, // top
            1, 2, 3, 1, 0, 3, // bottom
            4, 7, 6, 4, 5, 6  // back
        };

        class Cube : public Core::Node::Node
        {
        public:
            Cube() :
                m_vertexBuffer(s_CubeVertices, 8 * 3 * sizeof(GLfloat)),
                m_indexBuffer(s_CubeIndices, 36)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Cube() = default;

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
