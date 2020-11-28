#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Math/Math.h"
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class ShaderSrc;

        class CAERULUS_GRAPHICS Shader : public Core::Interface::NonCopyable
        {
        public:
            Shader(std::shared_ptr<ShaderSrc> vertex, std::shared_ptr<ShaderSrc> fragment);
            ~Shader() = default;

            void Bind() const;
            void Unbind() const;

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
            std::shared_ptr<ShaderSrc> m_vertex;
            std::shared_ptr<ShaderSrc> m_fragment;
            mutable std::unordered_map<std::string, int> m_uniformLocationCache;
        };
    }
}