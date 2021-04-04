#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Math/Math.h"
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
        class VertexArray;
        class IndexBuffer;

        class CAERULUS_GRAPHICS IRenderer : public Core::Interface::NonCopyable
        {
        public:
            virtual ~IRenderer() = default;

            virtual void Clear(Core::Math::vec4 colour) const = 0;
            virtual void Draw(const VertexArray& va, const IndexBuffer& ib, bool wireframe = false) const = 0;

            virtual void DrawSphere(Core::Math::vec3 position, double radius, Core::Math::vec3 colour) const = 0;
            virtual void DrawRay(Core::Math::vec3 start, Core::Math::vec3 end, Core::Math::vec3 colour, float lineWidth = 1.0) const = 0;
            virtual void DrawLine(Core::Math::vec3 point1, Core::Math::vec3 point2, Core::Math::vec3 colour, float lineWidth = 1.0) const = 0;
        };
    }
}