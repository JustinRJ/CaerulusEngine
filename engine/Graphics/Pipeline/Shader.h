#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "ShaderSource.h"
#include "../../Core/Math/MathHelper.h"

using namespace Core::Math;

namespace Graphics
{
    namespace PipeLine
    {

        class CAERULUS_GRAPHICS Shader
        {
        public:
            Shader(const ShaderSource& vertex, const ShaderSource& fragment);
            ~Shader();

            bool IsLinked() const;
            bool Link();
            void Use();
            void Unuse();

            GLuint GetHandle() const;
            
            void Set1i(const std::string& name, GLint value);
            void Set1f(const std::string& name, GLfloat value);
            void SetVec2f(const std::string& name, fvec2 value);
            void SetVec3f(const std::string& name, fvec3 value);
            void SetVec4f(const std::string& name, fvec4 value);
            void SetMat3fv(const std::string& name, mat3 value, GLboolean transpose = GL_FALSE);
            void SetMat4fv(const std::string& name, mat4 value, GLboolean transpose = GL_FALSE);

        private:
            bool m_IsLinked;
            GLuint m_Handle;
            ShaderSource m_Vertex;
            ShaderSource m_Fragment;
        };
    }
}