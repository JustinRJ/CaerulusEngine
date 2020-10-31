#pragma once

#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace Graphics
{
    namespace Geometry
    {
        const static GLfloat s_PlaneVertices[] =
        {
            -0.5, 0., -0.5, // bottom left 
             0.5, 0., -0.5, // bottom right
             0.5, 0.,  0.5, // top right   
            -0.5, 0.,  0.5  // top left    
        };

        const static GLuint s_PlaneIndices[] =
        {
            0, 3, 2,    0, 1, 2
        };

        class Plane
        {
        public:
            Plane() :
                m_vertexBuffer(s_PlaneVertices, 4 * 3 * sizeof(GLfloat)),
                m_indexBuffer(s_PlaneIndices, 6)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                m_vertexArray.AddBuffer(m_vertexBuffer, layout);
            }

            ~Plane() = default;

        private:
            Pipeline::VertexArray m_vertexArray;
            Pipeline::VertexBuffer m_vertexBuffer;
            Pipeline::IndexBuffer m_indexBuffer;
        };
    }
}
