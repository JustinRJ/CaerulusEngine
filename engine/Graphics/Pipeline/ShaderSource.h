#pragma once



#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <vector>

namespace Graphics
{
    namespace PipeLine
    {
        enum ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER
        };

        class CAERULUS_GRAPHICS ShaderSource
        {
        public:
            ShaderSource(ShaderType type, const std::string& path);
            ~ShaderSource();

            bool IsCompiled() const;
            void Load();
            GLuint GetHandle() const;

            const std::vector<std::string>& GetUniforms() const;

        private:

            void Compile(const std::string& source);
            void SetUniforms(const std::string& source);

            bool m_IsCompiled;
            GLuint m_Handle;
            ShaderType m_Type;
            const std::string m_Path;
            std::vector<std::string> m_Uniforms;
        };
    }
}

