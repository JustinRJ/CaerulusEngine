#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <vector>
#include "Core/Interface/NonCopyable.h"

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

        class CAERULUS_GRAPHICS ShaderSrc : public Core::Interface::NonCopyable
        {
        public:
            ShaderSrc(ShaderType type, const std::string& path);
            virtual ~ShaderSrc();

            void Load();
            bool IsCompiled() const;
            GLuint GetHandle() const;

            const std::vector<std::string>& GetUniforms() const;

        private:

            void Compile(const std::string& source);
            void SetUniforms(const std::string& source);

            bool m_isCompiled;
            GLuint m_handle;
            ShaderType m_type;
            const std::string m_path;
            std::vector<std::string> m_uniforms;
        };
    }
}

