#pragma once

#include <glew.h>
#include <vector>

namespace Graphics
{
    namespace Shaders
    {
        enum ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER
        };

        typedef std::vector<std::pair<std::string, size_t>> ShaderParams[2];

        class ShaderSource
        {
        public:
            ShaderSource(ShaderType type, const std::string& path);
            ~ShaderSource();

            bool IsLoaded() const;
            void Load();
            GLuint GetHandle() const;

        private:
            GLuint m_Handle;
            ShaderType m_Type;
            bool m_IsLoaded;
            const std::string& m_Path;
            const ShaderParams m_UniformLocations[2];
        };
    }
}