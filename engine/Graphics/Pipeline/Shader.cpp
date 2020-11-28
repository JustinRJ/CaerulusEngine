#include "stdafx.h"

#include "Shader.h"

#include "ShaderSrc.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Pipeline
    {
        GLuint Shader::m_boundHandle;

        Shader::Shader(std::shared_ptr<ShaderSrc> vertex, std::shared_ptr<ShaderSrc> fragment) :
            m_vertex(vertex),
            m_fragment(fragment)
        {
            using Core::Logging::Log;
            const unsigned int logSize = 512;
            // Shader Program Compilation
            GLchar infoLog[logSize];
            m_handle = glCreateProgram();
            glAttachShader(m_handle, m_vertex->GetHandle());
            glAttachShader(m_handle, m_fragment->GetHandle());
            glLinkProgram(m_handle);

            GLint isLinkSuccessful = false;
            glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinkSuccessful);

            if (!isLinkSuccessful)
            {
                glGetProgramInfoLog(m_handle, logSize, NULL, infoLog);
                Log::LogError("Shader program linking failed!", infoLog);
            }

            m_isLinked = true;
        }

        void Shader::Bind() const
        {
            if (m_boundHandle != m_handle)
            {
                glUseProgram(m_handle);
                m_boundHandle = m_handle;
            }
        }

        void Shader::Unbind()
        {
            if (m_boundHandle != 0)
            {
                glUseProgram(0);
                m_boundHandle = 0;
            }
        }

        GLuint Shader::GetHandle() const
        {
            return m_handle;
        }

        bool Shader::IsLinked() const
        {
            return m_isLinked;
        }

        void Shader::Set1i(const std::string& name, GLint value) const
        {
            glUniform1i(GetUniformLocation(name), value);
        }

        void Shader::Set1f(const std::string& name, GLfloat value) const
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        void Shader::Set2f(const std::string& name, Core::Math::fvec2 value) const
        {
            glUniform2fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set3f(const std::string& name, const Core::Math::fvec3& value) const
        {
            glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set4f(const std::string& name, const Core::Math::fvec4& value) const
        {
            glUniform4fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::SetMat3fv(const std::string& name, const Core::Math::mat3& value, GLboolean transpose) const
        {
            glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        void Shader::SetMat4fv(const std::string& name, const Core::Math::mat4& value, GLboolean transpose) const
        {
            glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        int Shader::GetUniformLocation(const std::string& name) const
        {
            if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
            {
                return m_uniformLocationCache[name];
            }

            int location = glGetUniformLocation(m_handle, name.c_str());

            if (location == -1)
            {
                Core::Logging::Log::LogInDebug("Shader::GetUniformLocation: " + name + "does not exist!");
            }

            m_uniformLocationCache[name] = location;
            return location;
        }
    }
}