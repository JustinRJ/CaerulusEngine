#include "stdafx.h"

#include "Pipeline\ShaderSource.h"

#include <glew.h>
#include <fstream>
#include <sstream>

#include "Log/Log.h"

using namespace Core::Log;

namespace Graphics
{
    namespace Pipeline
    {
        ShaderSource::ShaderSource(ShaderType type, std::string_view path) :
            m_type(type),
            m_isCompiled(false),
            m_path(path)
        {}

        ShaderSource::~ShaderSource()
        {
            glDeleteShader(m_handle);
        }

        bool ShaderSource::IsCompiled() const
        {
            return m_isCompiled;
        }

        void ShaderSource::Load()
        {
            std::string code;
            std::ifstream shaderFile;

            shaderFile.exceptions(std::ifstream::badbit);

            try
            {
                shaderFile.open(m_path);
                std::stringstream shaderStream;
                shaderStream << shaderFile.rdbuf();
                code = shaderStream.str();
                shaderFile.close();

                Compile(code);
            }
            catch (std::ifstream::failure e)
            {
                LogException("Shader program linking failed!", e.what());
            }
        }

        void ShaderSource::Compile(std::string_view source)
        {
            const uint32_t logSize = 512;
            const GLchar* shaderCode = source.data();
            // Shaders compilation
            GLchar infoLog[logSize];
            GLint isCompileSuccess;

            m_handle = glCreateShader(m_type);
            glShaderSource(m_handle, 1, &shaderCode, NULL);
            glCompileShader(m_handle);
            glGetShaderiv(m_handle, GL_COMPILE_STATUS, &isCompileSuccess);

            if (isCompileSuccess)
            {
                m_isCompiled = true;
            }
            else
            {
                glGetShaderInfoLog(m_handle, logSize, NULL, infoLog);
                LogError("Shader " + std::string(m_path) + "compilation failed!", infoLog);
            }
        }

        uint32_t ShaderSource::GetHandle() const
        {
            return m_handle;
        }
    }
}