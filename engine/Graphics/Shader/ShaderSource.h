#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

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
            CAERULUS_GRAPHICS ShaderSource(ShaderType type, const std::string& path);
            CAERULUS_GRAPHICS ~ShaderSource();

            CAERULUS_GRAPHICS bool IsLoaded() const;
            CAERULUS_GRAPHICS void Load();
            CAERULUS_GRAPHICS GLuint GetHandle() const;

        private:
            GLuint m_Handle;
            ShaderType m_Type;
            bool m_IsLoaded;
            const std::string& m_Path;
            const ShaderParams m_UniformLocations[2];
        };
    }
}