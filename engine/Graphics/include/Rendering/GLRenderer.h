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

            void Clear(Core::Math::vec4 colour) override;
            void Draw(Pipeline::VertexArray& va, Pipeline::IndexBuffer& ib) override;
            void Draw(Geometry::GPUGeometry& geometry) override;

            void DrawSphere(Core::Math::vec3 position, double radius, Core::Math::vec3 colour) override;
            void DrawRay(Core::Math::vec3 start, Core::Math::vec3 end, Core::Math::vec3 colour, float lineWidth = 2.5f) override;
            void DrawLine(Core::Math::vec3 point1, Core::Math::vec3 point2, Core::Math::vec3 colour, float lineWidth = 2.5f) override;

            bool IsWireframeActive() const;
            void SetWireframeActive(bool wireframe);

        private:
            bool m_wireframe;
        };
    }
}