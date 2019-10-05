#include "stdafx.h"

#include "Shader.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Graphics
{
    namespace PipeLine
    {
        Shader::Shader()
        {
        }

        Shader::~Shader()
        {
        }

        void Shader::Load(const GLchar* vertexPath, const GLchar* fragmentPath)
        {
            // Shaders reading
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;

            vShaderFile.exceptions(std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::badbit);

            try
            {
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;

                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                vShaderFile.close();
                fShaderFile.close();

                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure e)
            {
                std::cerr << "Shader file not succesfully read!" << std::endl;
            }

            const GLchar* vShaderCode = vertexCode.c_str();
            const GLchar * fShaderCode = fragmentCode.c_str();

            // Shaders compilation
            GLchar infoLog[512];

            // Vertex Shader
            m_VertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(m_VertexShaderObject, 1, &vShaderCode, NULL);
            glCompileShader(m_VertexShaderObject);
            glGetShaderiv(m_VertexShaderObject, GL_COMPILE_STATUS, &m_IsVertexSuccess);

            if (!m_IsVertexSuccess)
            {
                glGetShaderInfoLog(m_VertexShaderObject, 512, NULL, infoLog);
                std::cerr << "Shader vertex compilation failed!\n" << infoLog << std::endl;
            }

            // Fragment Shader
            m_FragmentShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(m_FragmentShaderObject, 1, &fShaderCode, NULL);
            glCompileShader(m_FragmentShaderObject);
            glGetShaderiv(m_FragmentShaderObject, GL_COMPILE_STATUS, &m_IsFragmentSuccess);

            if (!m_IsFragmentSuccess)
            {
                glGetShaderInfoLog(m_FragmentShaderObject, 512, NULL, infoLog);
                std::cerr << "Shader fragment compilation failed!\n" << infoLog << std::endl;
            }

            // Shader Program
            m_ShaderProgram = glCreateProgram();
            glAttachShader(m_ShaderProgram, m_VertexShaderObject);
            glAttachShader(m_ShaderProgram, m_FragmentShaderObject);
            glLinkProgram(m_ShaderProgram);
            glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &m_IsLinkSuccess);

            if (!m_IsLinkSuccess)
            {
                glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
                std::cerr << "Shader program linking failed!\n" << infoLog << std::endl;
            }

            glDeleteShader(m_VertexShaderObject);
            glDeleteShader(m_FragmentShaderObject);

            if (m_IsVertexSuccess && m_IsFragmentSuccess && m_IsLinkSuccess)
            {
                m_IsLoaded = true;
            }
        }

        bool Shader::Compile(const ShaderSource& vertex, const ShaderSource& fragment)
        {
            // Shader Program Compilation
            GLchar infoLog[512];
            m_ShaderProgram = glCreateProgram();
            glAttachShader(m_ShaderProgram, vertex.GetHandle());
            glAttachShader(m_ShaderProgram, fragment.GetHandle());
            glLinkProgram(m_ShaderProgram);
            glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &m_IsLinkSuccess);

            if (!m_IsLinkSuccess)
            {
                glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
                std::cerr << "Shader program linking failed!\n" << infoLog << std::endl;
            }

            if (m_IsLinkSuccess)
            {
                m_IsLoaded = true;
            }

            return false;
        }

        void Shader::Use() const
        {
            glUseProgram(m_ShaderProgram);
        }

        GLuint Shader::GetHandle() const
        {
            return m_ShaderProgram;
        }

        bool Shader::IsLoaded() const
        {
            return m_IsLoaded;
        }
    }
}