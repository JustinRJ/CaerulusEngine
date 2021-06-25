#pragma once

#include "Graphics/Geometry/GPUGeometry.h"

namespace Graphics
{
    namespace Geometry
    {
        const static GLfloat s_CubeVertices[] =
        {
           -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            0.5,  0.5, -0.5,
           -0.5,  0.5, -0.5,
           -0.5, -0.5,  0.5,
            0.5, -0.5,  0.5,
            0.5,  0.5,  0.5,
           -0.5,  0.5,  0.5
        };

        const static GLuint s_CubeIndices[] =
        {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
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
