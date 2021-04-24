#pragma once

#include "Graphics/Geometry/GPUGeometry.h"

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

        class Cube : public GPUGeometry
        {
        public:
            Cube() :
                GPUGeometry(s_CubeVertices, 8 * 3 * sizeof(GLfloat), s_CubeIndices, 36)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                GetVertexArray().AddBuffer(GetVertexBuffer(), layout);
            }

            ~Cube() = default;
        };
    }
}
