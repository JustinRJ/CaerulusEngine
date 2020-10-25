#include "stdafx.h"

#include "ShaderSrc.h"

#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace PipeLine
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
            // Shaders reading
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
                // Find uniforms
                SetUniforms(source);
                m_isCompiled = true;
            }
            else
            {
                using Core::Logging::Log;
                glGetShaderInfoLog(m_handle, logSize, NULL, infoLog);
                Log::LogError("Shader " + std::string(m_path) + "compilation failed!", infoLog);
            }
        }

        GLuint ShaderSrc::GetHandle() const
        {
            return m_handle;
        }

        void ShaderSrc::SetUniforms(const std::string& source)
        {
            auto shaderTypeToString = [&](ShaderType s) -> std::string
            {
                if (s == GL_VERTEX_SHADER)
                    return "Vertex Shader";
                else if (s == GL_FRAGMENT_SHADER)
                    return "Fragment Shader";
                else if (s == GL_GEOMETRY_SHADER)
                    return "Geometry Shader";
                else
                    return "Unknown Shader";
            };

            using namespace Core::Logging;
            Log::LogMessage("\t\tShaderType: " + shaderTypeToString(m_type));

            size_t offset = 0U;
            bool searchForUniforms = true;
            while (searchForUniforms)
            {
                auto spaceBeforeName = source.find("uniform", offset);
                if (spaceBeforeName != std::string::npos)
                {
                    spaceBeforeName = source.find(" ", source.find(" ", spaceBeforeName) + 1);
                    auto semicolonAfterName = source.find(";", spaceBeforeName);
                    std::string unfiformName = std::string(source.c_str() + spaceBeforeName, source.c_str() + semicolonAfterName);
                    offset = semicolonAfterName;
                    m_uniforms.push_back(unfiformName);
                    Log::LogMessage("\t\tUniform " + std::to_string(m_uniforms.size()) + ": " + unfiformName);
                }
                else
                {
                    searchForUniforms = false;
                }
            }
        }

        const std::vector<std::string>& ShaderSrc::GetUniforms() const
        {
            return m_uniforms;
        }
    }
}