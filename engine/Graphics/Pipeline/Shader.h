#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Math/Math.h"
#include "Core/Logging/Log.h"
#include "IBindable.h"

namespace Graphics
{
    namespace PipeLine
    {
        class ShaderSrc;
    }
}

namespace Graphics
{
    using namespace Core::Math;

    namespace PipeLine
    {
        class CAERULUS_GRAPHICS Shader : public IBindable
        {
        public:
            Shader(std::shared_ptr<ShaderSrc> vertex, std::shared_ptr<ShaderSrc> fragment);
            virtual ~Shader() = default;

            GLuint GetHandle() const
            {
                return m_handle;
            }

            bool IsLinked() const
            {
                return m_isLinked;
            }

            void Bind() const
            {
                glUseProgram(m_handle);
            }

            void Unbind() const
            {
                glUseProgram(0);
            }

            void Set1i(const std::string& name, GLint value)
            {
                glUniform1i(GetUniformLocation(name), value);
            }

            void Set1f(const std::string& name, GLfloat value)
            {
                glUniform1f(GetUniformLocation(name), value);
            }

            void Set2f(const std::string& name, fvec2 value)
            {
                glUniform2fv(GetUniformLocation(name), 1, value_ptr(value));
            }

            void Set3f(const std::string& name, fvec3 value)
            {
                glUniform3fv(GetUniformLocation(name), 1, value_ptr(value));
            }

            void Set4f(const std::string& name, fvec4 value)
            {
                glUniform4fv(GetUniformLocation(name), 1, value_ptr(value));
            }

            void SetMat3fv(const std::string& name, mat3 value, GLboolean transpose = GL_FALSE)
            {
                glUniformMatrix3fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
            }

            void SetMat4fv(const std::string& name, mat4 value, GLboolean transpose = GL_FALSE)
            {
                glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, value_ptr(value));
            }

        private:

            int GetUniformLocation(const std::string& name)
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

            GLuint m_handle;
            bool m_isLinked = false;
            std::shared_ptr<ShaderSrc> m_vertex;
            std::shared_ptr<ShaderSrc> m_fragment;
            std::unordered_map<std::string, int> m_uniformLocationCache;
        };
    }
}