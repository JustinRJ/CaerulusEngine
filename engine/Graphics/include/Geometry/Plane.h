#pragma once

#include "Geometry/GPUGeometry.h"

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

        class Plane : public GPUGeometry
        {
        public:
            Plane(const Managers::ShaderManager& shaderManager) :
                GPUGeometry(shaderManager, s_PlaneVertices, 4 * 3 * sizeof(GLfloat), s_PlaneIndices, 6)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(3);
                GetVertexArray().AddBuffer(GetVertexBuffer(), layout);
            }
        };
    }
}
