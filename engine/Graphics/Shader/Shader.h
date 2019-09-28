#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "ShaderSource.h"

namespace Graphics
{
    namespace Shaders
    {
        class CAERULUS_GRAPHICS Shader
        {
        public:
            Shader();
            ~Shader();

            bool IsLoaded() const;
            void Load(const GLchar* vertexPath, const GLchar* fragmentPath);
            bool Compile(const ShaderSource& vertex, const ShaderSource& fragment);
            void Use() const;

            GLuint GetHandle() const;

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