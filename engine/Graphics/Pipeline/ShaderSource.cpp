#include "stdafx.h"

#include "ShaderSource.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace Graphics
{
    namespace PipeLine
    {
        ShaderSource::ShaderSource(ShaderType type, const std::string& path) :
            m_Type(type),
            m_IsLoaded(false),
            m_Path(path)
        {
        }

        ShaderSource::~ShaderSource()
        {
            glDeleteShader(m_Handle);
        }

        bool ShaderSource::IsLoaded() const
        {
            return m_IsLoaded;
        }

        void ShaderSource::Load()
        {
            // Shaders reading
            std::string code;
            std::ifstream shaderFile;

            shaderFile.exceptions(std::ifstream::badbit);

            try
            {
                shaderFile.open(m_Path);
                std::stringstream shaderStream;
                shaderStream << shaderFile.rdbuf();
                shaderFile.close();
                code = shaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                std::cerr << "Shader file not succesfully read!" << std::endl;
            }

            const GLchar* shaderCode = code.c_str();

            // Shaders compilation
            GLchar infoLog[512];
            GLint isLinkSuccess;

            m_Handle = glCreateShader(m_Type);
            glShaderSource(m_Handle, 1, &shaderCode, NULL);
            glCompileShader(m_Handle);
            glGetShaderiv(m_Handle, GL_COMPILE_STATUS, &isLinkSuccess);

            if (!isLinkSuccess)
            {
                glGetShaderInfoLog(m_Handle, 512, NULL, infoLog);
                std::cerr << "Shader " + std::string(m_Path) + "compilation failed!\n" << infoLog << std::endl;
            }
            else
            {
                m_IsLoaded = true;
            }
        }

        GLuint ShaderSource::GetHandle() const
        {
            return m_Handle;
        }
    }
}