#include "stdafx.h"

#include "Shader.h"
#include "../../Core/Logging/Log.h"

namespace Graphics
{
    namespace PipeLine
    {
        Shader::Shader(const ShaderSource& vertex, const ShaderSource& fragment) :
            m_Vertex(vertex),
            m_Fragment(fragment),
            m_SetUniforms({}),
            m_UpdateUniforms({})
        {
        }

        bool Shader::Link()
        {
            using Core::Logging::Log;
            const unsigned int logSize = 512;
            // Shader Program Compilation
            GLchar infoLog[logSize];
            m_Handle = glCreateProgram();
            glAttachShader(m_Handle, m_Vertex.GetHandle());
            glAttachShader(m_Handle, m_Fragment.GetHandle());
            glLinkProgram(m_Handle);

            GLint isLinkSuccessful = false;
            glGetProgramiv(m_Handle, GL_LINK_STATUS, &isLinkSuccessful);

            if (!isLinkSuccessful)
            {
                glGetProgramInfoLog(m_Handle, logSize, NULL, infoLog);
                Log::LogError("Shader program linking failed!", infoLog);
                return false;
            }

            m_IsLinked = true;
            return true;
        }

        GLuint Shader::GetHandle() const
        {
            return m_Handle;
        }

        void Shader::SetUniforms()
        {
            m_SetUniforms();
        }

        void Shader::UpdateUniforms()
        {
            m_UpdateUniforms();
        }

        void Shader::SetUniforms(std::function<void()> setUniforms)
        {
            m_SetUniforms = setUniforms;
        }

        void Shader::UpdateUniforms(std::function<void()> updateUniforms)
        {
            m_UpdateUniforms = updateUniforms;
        }

        bool Shader::IsLinked() const
        {
            return m_IsLinked;
        }
    }
}