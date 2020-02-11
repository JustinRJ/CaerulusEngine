#include "stdafx.h"

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace PipeLine;

        Geometry::Geometry() :
            m_Transform(mat4())
        {
            SetVertices();
        }

        Geometry::Geometry(const mat4& transform) :
            m_Transform(mat4(transform))
        {
            SetVertices();
        }

        void Geometry::SetTransform(const mat4& transform)
        {
            m_Transform = transform;
        }

        const mat4& Geometry::GetTransform() const
        {
            return m_Transform;
        }
    }
}