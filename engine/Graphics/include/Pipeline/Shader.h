#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <memory>
#include <unordered_map>

#include "Math/Math.h"
#include "Interface/NonCopyable.h"
#include "AssetManagers/ShaderSourceManager.h"

namespace Graphics
{
    namespace Pipeline
    {
        class ShaderSource;

        class CAERULUS_GRAPHICS Shader : Core::Interface::NonCopyable
        {
        public:
            Shader(const AssetManagers::ShaderSourceManager& shaderSourceManager, const std::string& vertex, const std::string& fragment);

            void Bind();
            static void Unbind();

            GLuint GetHandle() const;
            bool IsLinked() const;

            void Set1i(const std::string& name, GLint value);
            void Set1f(const std::string& name, GLfloat value);
            void Set2f(const std::string& name, Core::Math::fvec2 value);
            void Set3f(const std::string& name, const Core::Math::fvec3& value);
            void Set4f(const std::string& name, const Core::Math::fvec4& value);
            void SetMat3fv(const std::string& name, const Core::Math::mat3& value, GLboolean transpose = GL_FALSE);
            void SetMat4fv(const std::string& name, const Core::Math::mat4& value, GLboolean transpose = GL_FALSE);

        private:
            int GetUniformLocation(const std::string& name);

            GLuint m_handle;
            static GLuint m_boundHandle;
            bool m_isLinked = false;
            const std::string m_vertex;
            const std::string m_fragment;
            std::unordered_map<std::string, int> m_uniformLocationCache;

            const AssetManagers::ShaderSourceManager& m_shaderSourceManager;
        };
    }
}