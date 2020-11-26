#pragma once

#include "IRenderer.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS Renderer : public IRenderer
        {
        public:
            Renderer();
            ~Renderer() = default;

            void Clear(Core::Math::vec4 colour) override;
            void Draw(const VertexArray& va, const IndexBuffer& ib, bool wireframe = false) override;
        };
    }
}