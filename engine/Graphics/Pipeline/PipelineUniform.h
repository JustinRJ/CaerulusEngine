#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <string>
#include "../../Core/Math/MathHelper.h"

using namespace Core::Math;
namespace Graphics
{
    namespace PipeLine
    {
        template <class T>
        class CAERULUS_GRAPHICS PipelineUniform
        {
        public:
            virtual void SetUniforms()
            {
                if (auto t = dynamic_cast<T*>(this))
                {
                    m_SetUniforms(*t);
                }
            }

            virtual void UpdateUniforms()
            {
                if (auto t = dynamic_cast<T*>(this))
                {
                    m_UpdateUniforms(*t);
                }
            }

            virtual void SetUniformsCallback(std::function<void(T&)> setUniforms)
            {
                m_SetUniforms = setUniforms;
            }

            virtual void UpdateUniformsCallback(std::function<void(T&)> updateUniforms)
            {
                m_UpdateUniforms = updateUniforms;
            }

            virtual void Bind()
            {
                glUseProgram(m_Handle);
            }

            virtual void Unbind()
            {
                glUseProgram(0);
            }

            void Set1i(const std::string& name, GLint value)
            {
                Bind();
                glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), value);
            }

            void Set1f(const std::string& name, GLfloat value)
            {
                Bind();
                glUniform1f(glGetUniformLocation(m_Handle, name.c_str()), value);
            }

            void Set2f(const std::string& name, fvec2 value)
            {
                Bind();
                glUniform2fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            }

            void Set3f(const std::string& name, fvec3 value)
            {
                Bind();
                glUniform3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            }

            void Set4f(const std::string & name, fvec4 value)
            {
                Bind();
                glUniform4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, value_ptr(value));
            }

            void SetMat3fv(const std::string & name, mat3 value, GLboolean transpose = GL_FALSE)
            {
                Bind();
                glUniformMatrix3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, transpose, value_ptr(value));
            }

            void SetMat4fv(const std::string & name, mat4 value, GLboolean transpose = GL_FALSE)
            {
                Bind();
                glUniformMatrix4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, transpose, value_ptr(value));
            }

        protected:
            GLuint m_Handle;
            std::function<void(T&)> m_SetUniforms = {};
            std::function<void(T&)> m_UpdateUniforms = {};
        };
    }
}