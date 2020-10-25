#pragma once

#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace Graphics
{
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
                PipeLine::VertexBufferLayout layout;
                layout.Push<float>(2);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Quad() = default;

            void Draw(bool wireframe)
            {
                m_vertexArray.Bind();
                m_indexBuffer.Bind();
                glDrawElements(wireframe ? GL_LINE_LOOP : GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }

        private:
            PipeLine::VertexArray m_vertexArray;
            PipeLine::VertexBuffer m_vertexBuffer;
            PipeLine::IndexBuffer m_indexBuffer;
        };
    }
}
