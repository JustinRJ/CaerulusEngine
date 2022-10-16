#include "stdafx.h"

#include "Pipeline/Shader.h"

#include "Log/Log.h"
#include "Pipeline/ShaderSource.h"

using namespace Core::Log;

namespace Graphics
{
    namespace Pipeline
    {
        Shader::Shader(const AssetManagers::ShaderSourceManager& shaderSourceManager,
            std::string_view vertex, std::string_view fragment) :
            m_vertex(vertex),
            m_fragment(fragment),
            m_shaderSourceManager(shaderSourceManager),
            m_isLinked(false)
        {
            const uint32_t LogSize = 512;
            // Shader Program Compilation
            GLchar infoLog[LogSize];
            m_handle = glCreateProgram();
            glAttachShader(m_handle, m_shaderSourceManager.Get(m_vertex)->GetHandle());
            glAttachShader(m_handle, m_shaderSourceManager.Get(fragment.data())->GetHandle());
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

        void Shader::Set1i(std::string_view name, GLint value)
        {
            glUniform1i(GetUniformLocation(name), value);
        }

        void Shader::Set1f(std::string_view name, GLfloat value)
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        void Shader::Set2f(std::string_view name, Core::Math::fvec2 value)
        {
            glUniform2fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set3f(std::string_view name, const Core::Math::fvec3& value)
        {
            glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::Set4f(std::string_view name, const Core::Math::fvec4& value)
        {
            glUniform4fv(GetUniformLocation(name), 1, value_ptr(value));
        }

        void Shader::SetMat3fv(std::string_view name, const Core::Math::mat3& value, GLboolean transpose)
        {
            glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        void Shader::SetMat4fv(std::string_view name, const Core::Math::mat4& value, GLboolean transpose)
        {
            glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
        }

        int Shader::GetUniformLocation(std::string_view name)
        {
            if (m_uniformLocationCache.find(name.data()) != m_uniformLocationCache.end())
            {
                return m_uniformLocationCache[name.data()];
            }

            int location = glGetUniformLocation(m_handle, name.data());

            if (location == -1)
            {
                LogInDebug("Shader::GetUniformLocation: " + std::string(name) + "does not exist!");
            }

            m_uniformLocationCache[name.data()] = location;
            return location;
        }
    }
}