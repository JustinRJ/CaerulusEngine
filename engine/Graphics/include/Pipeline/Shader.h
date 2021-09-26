#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <memory>
#include <unordered_map>

#include "Math/Math.h"
#include "Interface/NonCopyable.h"
#include "Managers/ShaderSourceManager.h"

namespace Graphics
{
    namespace Pipeline
    {
        class ShaderSource;

        class CAERULUS_GRAPHICS Shader : Core::Interface::NonCopyable
        {
        public:
            Shader(const Managers::ShaderSourceManager& shaderSourceManager, const std::string& vertex, const std::string& fragment);

            void Bind() const;
            static void Unbind();

            GLuint GetHandle() const;
            bool IsLinked() const;

            void Set1i(const std::string& name, GLint value) const;
            void Set1f(const std::string& name, GLfloat value) const;
            void Set2f(const std::string& name, Core::Math::fvec2 value) const;
            void Set3f(const std::string& name, const Core::Math::fvec3& value) const;
            void Set4f(const std::string& name, const Core::Math::fvec4& value) const;
            void SetMat3fv(const std::string& name, const Core::Math::mat3& value, GLboolean transpose = GL_FALSE) const;
            void SetMat4fv(const std::string& name, const Core::Math::mat4& value, GLboolean transpose = GL_FALSE) const;

        private:
            int GetUniformLocation(const std::string& name) const;

            GLuint m_handle;
            static GLuint m_boundHandle;
            bool m_isLinked = false;
            const std::string m_vertex;
            const std::string m_fragment;
            mutable std::unordered_map<std::string, int> m_uniformLocationCache;

            const Managers::ShaderSourceManager& m_shaderSourceManager;
        };
    }
}