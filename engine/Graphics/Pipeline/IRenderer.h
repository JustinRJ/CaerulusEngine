#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Core
{
    namespace Math
    {
        class Camera;
    }
}

namespace Graphics
{
    namespace Window
    {
        class GLWindow;
    }

    namespace Pipeline
    {
        class Shader;
        class VertexArray;
        class IndexBuffer;

        class CAERULUS_GRAPHICS IRenderer : public Core::Interface::NonCopyable
        {
        public:

            virtual void Clear(glm::vec4 colour) = 0;
            virtual void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, bool wireframe = false) = 0;
        };
    }
}