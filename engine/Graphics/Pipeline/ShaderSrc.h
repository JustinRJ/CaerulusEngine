#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <string>
#include <glew.h>

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        enum ShaderType
        {
            Vertex = GL_VERTEX_SHADER,
            Fragment = GL_FRAGMENT_SHADER,
            Geometry = GL_GEOMETRY_SHADER
        };

        class CAERULUS_GRAPHICS ShaderSrc : public Core::Interface::NonCopyable
        {
        public:
            ShaderSrc(ShaderType type, const std::string& path);
            ~ShaderSrc();

            void Load();
            bool IsCompiled() const;
            unsigned int GetHandle() const;

        private:
            void Compile(const std::string& source);

            bool m_isCompiled;
            GLuint m_handle;
            ShaderType m_type;
            const std::string m_path;
        };
    }
}

