#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport) 

#include "Geometry.h"

namespace Graphics
{
    namespace Geometry
    {
        class CubeGeometry : public Geometry
        {
        public:
            static const GLfloat s_CubeVertices[];

            CAERULUS_GRAPHICS CubeGeometry();
            CAERULUS_GRAPHICS CubeGeometry(const mat4& transform);
            CAERULUS_GRAPHICS virtual ~CubeGeometry();

            CAERULUS_GRAPHICS void Draw() const override;

        private:
            virtual void GenerateBuffer() override;
        };
    }
}
