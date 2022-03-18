#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Math/Math.h"
#include "Interface/NonCopyable.h"

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

    namespace Geometry
    {
        class GPUGeometry;
    }

    namespace Pipeline
    {
        class VertexArray;
        class IndexBuffer;
    }

    namespace Rendering
    {
        class CAERULUS_GRAPHICS IRenderer : Core::Interface::NonCopyable
        {
        public:
            virtual ~IRenderer() = default;

            virtual void Clear(Core::Math::vec4 colour) = 0;
            virtual void Draw(Pipeline::VertexArray& va, Pipeline::IndexBuffer& ib) = 0;
            virtual void Draw(Geometry::GPUGeometry& geometry) = 0;

            virtual void DrawSphere(Core::Math::vec3 position, double radius, Core::Math::vec3 colour) = 0;
            virtual void DrawRay(Core::Math::vec3 start, Core::Math::vec3 end, Core::Math::vec3 colour, float lineWidth = 1.0) = 0;
            virtual void DrawLine(Core::Math::vec3 point1, Core::Math::vec3 point2, Core::Math::vec3 colour, float lineWidth = 1.0) = 0;
        };
    }
}