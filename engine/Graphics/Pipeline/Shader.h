#pragma once

#include "PiplineUniform.h"
#include "ShaderSource.h"
#include "../../Core/Math/MathHelper.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS Shader : public PiplineUniform
        {
        public:
            Shader(const ShaderSource& vertex, const ShaderSource& fragment);
            virtual ~Shader() = default;

            void SetUniforms() override;
            void UpdateUniforms() override;

            bool Link();
            bool IsLinked() const;
            GLuint GetHandle() const;

            void SetUniforms(std::function<void()> setUniforms);
            void UpdateUniforms(std::function<void()> updateUniforms);

        private:
            bool m_IsLinked;
            ShaderSource m_Vertex;
            ShaderSource m_Fragment;

            std::function<void()> m_SetUniforms;
            std::function<void()> m_UpdateUniforms;
        };
    }
}