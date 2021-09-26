#pragma once

#include "Geometry/GPUGeometry.h"

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

        class Quad : public GPUGeometry
        {
        public:
            Quad(Core::Node::Node* parent, const Managers::ShaderManager& shaderManager) :
                GPUGeometry(parent, shaderManager, s_QuadVertices, 4 * 2 * sizeof(GLfloat), s_QuadIndices, 6)
            {
                Pipeline::VertexBufferLayout layout;
                layout.Push<float>(2);
                GetVertexArray().AddBuffer(GetVertexBuffer(), layout);
            }
        };
    }
}
