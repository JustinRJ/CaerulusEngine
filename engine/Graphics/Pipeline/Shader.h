#pragma once

#include "PipelineUniform.h"

namespace Graphics
{
    namespace PipeLine
    {
        class ShaderSrc;
    }
}

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS Shader : public PipelineUniform
        {
        public:
            Shader(std::shared_ptr<ShaderSrc> vertex, std::shared_ptr<ShaderSrc> fragment);
            virtual ~Shader() = default;

            bool Link();
            bool IsLinked() const;
            GLuint GetHandle() const;

        private:
            bool m_isLinked;
            std::shared_ptr<ShaderSrc> m_vertex;
            std::shared_ptr<ShaderSrc> m_fragment;
        };
    }
}