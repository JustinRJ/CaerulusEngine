#include "stdafx.h"

#include "Pipeline/Shader.h"

#include "Logging/Log.h"
#include "Pipeline/ShaderSource.h"

using namespace Core::Logging;

namespace Graphics
{
    namespace Pipeline
    {
        Shader::Shader(const AssetManagers::ShaderSourceManager& shaderSourceManager,
            const std::string& vertex, const std::string& fragment) :
            m_vertex(vertex),
            m_fragment(fragment),
            m_shaderSourceManager(shaderSourceManager),
            m_isLinked(false)
        {
            const unsigned int LogSize = 512;
            // Shader Program Compilation
            GLchar infoLog[LogSize];
            m_handle = glCreateProgram();
            glAttachShader(m_handle, m_shaderSourceManager.Get(m_vertex)->GetHandle());
            glAttachShader(m_handle, m_shaderSourceManager.Get(fragment)->GetHandle());
            glLinkProgram(m_handle);

            GLint isLinkSuccessful = false;
            glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinkSuccessful);

            if (!isLinkSuccessful)
            {
                glGetProgramInfoLog(m_handle, LogSize, NULL, infoLog);
                LogError("Shader program linking failed!", infoLog);
            }

            m_isLinked = static_cast<bool>(isLinkSuccessful);
        }

        void Shader::Bind()
        {
            glUseProgram(m_handle);
        }

        void Shader::Unbind()
        {
            glUseProgram(0);
        }

        GLuint Shader::GetHandle() const
        {
            return m_handle;
        }

        bool Shader::IsLinked() const
        {
            return m_isLinked;
        }

        void Shader::Set1i(const std::string& name, GLint value)
        {
            glUniform1i(GetUniformLocation(name), value);
        }

        void Shader::Set1f(const std::string& name, GLfloat value)
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        void Shader::Set2f(const std::string& name, Core::Math::fvec2 value)
        {
            glUniform2fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set3f(const std::string& name, const Core::Math::fvec3& value)
        {
            glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set4f(const std::string& name, const Core::Math::fvec4& value)
        {
            glUniform4fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::SetMat3fv(const std::string& name, const Core::Math::mat3& value, GLboolean transpose)
        {
            glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        void Shader::SetMat4fv(const std::string& name, const Core::Math::mat4& value, GLboolean transpose)
        {
            glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        int Shader::GetUniformLocation(const std::string& name)
        {
            if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end())
            {
                return m_uniformLocationCache[name];
            }

            int location = glGetUniformLocation(m_handle, name.c_str());

            if (location == -1)
            {
                LogInDebug("Shader::GetUniformLocation: " + name + "does not exist!");
            }

            m_uniformLocationCache[name] = location;
            return location;
        }
    }
}