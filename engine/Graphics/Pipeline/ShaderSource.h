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

        typedef std::vector<std::pair<std::string, size_t>> ShaderParams[2];

        class CAERULUS_GRAPHICS ShaderSource
        {
        public:
            ShaderSource(ShaderType type, const std::string& path);
            ~ShaderSource();

            bool IsLoaded() const;
            void Load();
            GLuint GetHandle() const;

        private:
            bool m_IsLoaded;
            GLuint m_Handle;
            ShaderType m_Type;
            const std::string m_Path;
            const ShaderParams m_UniformLocations[2];
        };
    }
}

