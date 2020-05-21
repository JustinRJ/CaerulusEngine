#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <string>
#include <functional>
#include "Core/Math/Math.h"
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS PipelineUniform : public Core::Interface::NonCopyable
        {
        public:
            PipelineUniform() = default;
            virtual ~PipelineUniform() = default;

            virtual void Bind()
            {
                glUseProgram(m_handle);
            }

            virtual void Unbind()
            {
                glUseProgram(0);
            }

            void Set1i(const std::string& name, GLint value)
            {
                Bind();
                glUniform1i(glGetUniformLocation(m_handle, name.c_str()), value);
            }

            void Set1f(const std::string& name, GLfloat value)
            {
                Bind();
                glUniform1f(glGetUniformLocation(m_handle, name.c_str()), value);
            }

            void Set2f(const std::string& name, fvec2 value)
            {
                Bind();
                glUniform2fv(glGetUniformLocation(m_handle, name.c_str()), 1, value_ptr(value));
            }

            void Set3f(const std::string& name, fvec3 value)
            {
                Bind();
                glUniform3fv(glGetUniformLocation(m_handle, name.c_str()), 1, value_ptr(value));
            }

            void Set4f(const std::string& name, fvec4 value)
            {
                Bind();
                glUniform4fv(glGetUniformLocation(m_handle, name.c_str()), 1, value_ptr(value));
            }

            void SetMat3fv(const std::string& name, mat3 value, GLboolean transpose = GL_FALSE)
            {
                Bind();
                glUniformMatrix3fv(glGetUniformLocation(m_handle, name.c_str()), 1, transpose, value_ptr(value));
            }

            void SetMat4fv(const std::string& name, mat4 value, GLboolean transpose = GL_FALSE)
            {
                Bind();
                glUniformMatrix4fv(glGetUniformLocation(m_handle, name.c_str()), 1, transpose, value_ptr(value));
            }

        protected:
            GLuint m_handle;
        };
    }
}