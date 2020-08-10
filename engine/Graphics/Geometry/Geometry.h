#pragma once

#include "Graphics/Pipeline/PipelineVertex.h"
#include "Core/Math/Math.h"

namespace Graphics
{
    namespace Geometry
    {
        using namespace Core::Math;
        class CAERULUS_GRAPHICS Geometry : public PipeLine::PiplineVertex
        {
        public:
            Geometry();
            Geometry(const mat4& transform);
            virtual ~Geometry() = default;

            virtual void Draw(bool wireframe) const = 0;

            void SetTransform(const mat4& transform);
            const mat4& GetTransform() const;

        protected:
            mat4 m_transform;
        };
    }
}
