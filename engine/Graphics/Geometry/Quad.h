#pragma once

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class CAERULUS_GRAPHICS Quad : public Geometry
        {
        public:
            static const GLfloat s_QuadVertices[];

            Quad();
            Quad(const mat4& transform);
            virtual ~Quad() = default;

            void Draw(bool wireframe) const override;

        private:
            void SetVertices() override;
        };
    }
}
