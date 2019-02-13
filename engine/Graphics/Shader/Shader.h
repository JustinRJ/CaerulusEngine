#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "ShaderSource.h"

namespace Graphics
{
    namespace Shaders
    {
        class Shader
        {
        public:
            CAERULUS_GRAPHICS Shader();
            CAERULUS_GRAPHICS ~Shader();

            CAERULUS_GRAPHICS bool IsLoaded() const;
            CAERULUS_GRAPHICS void Load(const GLchar* vertexPath, const GLchar* fragmentPath);
            CAERULUS_GRAPHICS bool Compile(const ShaderSource& vertex, const ShaderSource& fragment);
            CAERULUS_GRAPHICS void Use() const;

            CAERULUS_GRAPHICS GLuint GetHandle() const;

        private:
            GLuint m_ShaderProgram;
            GLuint m_VertexShaderObject;
            GLuint m_FragmentShaderObject;

            bool m_IsLoaded;
            GLint m_IsVertexSuccess;
            GLint m_IsFragmentSuccess;
            GLint m_IsLinkSuccess;
        };
    }
}