#pragma once

#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace Graphics
{
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

        class Cube
        {
        public:
            Cube() :
                m_vertexBuffer(s_CubeVertices, 8 * 3 * sizeof(GLfloat)),
                m_indexBuffer(s_CubeIndices, 36)
            {
                PipeLine::VertexBufferLayout layout;
                layout.Push<float>(3);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Cube() = default;

            void Draw(bool wireframe)
            {
                m_vertexArray.Bind();
                m_indexBuffer.Bind();
                glDrawElements(wireframe ? GL_LINE_LOOP : GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
            }

        private:
            PipeLine::VertexArray m_vertexArray;
            PipeLine::VertexBuffer m_vertexBuffer;
            PipeLine::IndexBuffer m_indexBuffer;
        };
    }
}
