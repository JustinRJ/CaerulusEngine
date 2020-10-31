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
            virtual ~Renderer() = default;

            void Clear(glm::vec4 colour) override;
            void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, bool wireframe = false) override;
        };
    }
}