#pragma once

#include "PipelineUniform.h"
#include "ShaderSrc.h"
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS Shader : public PipelineUniform<Shader>
        {
        public:
            Shader(const ShaderSrc& vertex, const ShaderSrc& fragment);
            virtual ~Shader() = default;

            bool Link();
            bool IsLinked() const;
            GLuint GetHandle() const;

        private:
            bool m_IsLinked;
            ShaderSrc m_Vertex;
            ShaderSrc m_Fragment;
        };
    }
}