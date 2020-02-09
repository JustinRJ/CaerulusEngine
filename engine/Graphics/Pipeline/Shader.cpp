#include "stdafx.h"

#include "Shader.h"
#include "../../Core/Logging/Log.h"

namespace Graphics
{
    namespace PipeLine
    {
        Shader::Shader(const ShaderSource& vertex, const ShaderSource& fragment) :
            m_Vertex(vertex),
            m_Fragment(fragment)
        {
        }

        Shader::~Shader()
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

        void Shader::Use()
        {
            glUseProgram(m_Handle);
        }

        void Shader::Unuse()
        {
            glUseProgram(0);
        }

        GLuint Shader::GetHandle() const
        {
            return m_Handle;
        }

        void Shader::Set1i(const std::string& name, GLint value)
        {
            Use();
            glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), value);
            Unuse();
        }

        void Shader::Set1f(const std::string& name, GLfloat value)
        {
            Use();
            glUniform1f(glGetUniformLocation(m_Handle, name.c_str()), value);
            Unuse();
        }

        void Shader::SetVec2f(const std::string& name, fvec2 value)
        {
            Use();
            glUniform2fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            Unuse();
        }

        void Shader::SetVec3f(const std::string& name, fvec3 value)
        {
            Use();
            glUniform3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            Unuse();
        }

        void Shader::SetVec4f(const std::string & name, fvec4 value)
        {
            Use();
            glUniform4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            Unuse();
        }

        void Shader::SetMat3fv(const std::string & name, mat3 value, GLboolean transpose)
        {
            Use();
            glUniformMatrix3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, transpose, value_ptr(value));
            Unuse();
        }

        void Shader::SetMat4fv(const std::string & name, mat4 value, GLboolean transpose)
        {
            Use();
            glUniformMatrix4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, transpose, value_ptr(value));
            Unuse();
        }

        bool Shader::IsLinked() const
        {
            return m_IsLinked;
        }
    }
}