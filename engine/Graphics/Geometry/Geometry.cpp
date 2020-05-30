#include "stdafx.h"

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace PipeLine;

        Geometry::Geometry() :
            m_transform(mat4())
        {
            Init();
        }

        Geometry::Geometry(const mat4& transform) :
            m_transform(mat4(transform))
        {
            Init();
        }

        void Geometry::SetTransform(const mat4& transform)
        {
            m_transform = transform;
        }

        const mat4& Geometry::GetTransform() const
        {
            return m_transform;
        }
    }
}