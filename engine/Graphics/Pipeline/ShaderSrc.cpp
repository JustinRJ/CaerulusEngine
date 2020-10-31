#include "stdafx.h"

#include "ShaderSrc.h"

#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Pipeline
    {
        ShaderSrc::ShaderSrc(ShaderType type, const std::string& path) :
            m_type(type),
            m_isCompiled(false),
            m_path(path)
        {
        }

        ShaderSrc::~ShaderSrc()
        {
            glDeleteShader(m_handle);
        }

        bool ShaderSrc::IsCompiled() const
        {
            return m_isCompiled;
        }

        void ShaderSrc::Load()
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
                using Core::Logging::Log;
                Log::LogException("Shader program linking failed!", e.what());
            }
        }

        void ShaderSrc::Compile(const std::string& source)
        {
            const unsigned int logSize = 512;
            const GLchar* shaderCode = source.c_str();
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
                using Core::Logging::Log;
                glGetShaderInfoLog(m_handle, logSize, NULL, infoLog);
                Log::LogError("Shader " + std::string(m_path) + "compilation failed!", infoLog);
            }
        }

        unsigned int ShaderSrc::GetHandle() const
        {
            return m_handle;
        }
    }
}