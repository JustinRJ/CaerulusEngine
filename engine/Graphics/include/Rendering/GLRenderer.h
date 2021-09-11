#pragma once

#include "IRenderer.h"

namespace Graphics
{
    namespace Rendering
    {
        class CAERULUS_GRAPHICS GLRenderer : public IRenderer
        {
        public:
            GLRenderer();

            void Clear(Core::Math::vec4 colour) const override;
            void Draw(const Pipeline::VertexArray& va, const Pipeline::IndexBuffer& ib, bool wireframe = false) const override;
            void Draw(const Geometry::GPUGeometry& geometry, bool wireframe = false) const override;

            void DrawSphere(Core::Math::vec3 position, double radius, Core::Math::vec3 colour) const override;
            void DrawRay(Core::Math::vec3 start, Core::Math::vec3 end, Core::Math::vec3 colour, float lineWidth = 2.5f) const override;
            void DrawLine(Core::Math::vec3 point1, Core::Math::vec3 point2, Core::Math::vec3 colour, float lineWidth = 2.5f) const override;
        };
    }
}