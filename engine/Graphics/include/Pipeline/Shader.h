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
            Shader(const AssetManagers::ShaderSourceManager& shaderSourceManager, std::string_view vertex, std::string_view fragment);

            void Bind();
            static void Unbind();

            GLuint GetHandle() const;
            bool IsLinked() const;

            void Set1i(std::string_view name, GLint value);
            void Set1f(std::string_view name, GLfloat value);
            void Set2f(std::string_view name, Core::Math::fvec2 value);
            void Set3f(std::string_view name, const Core::Math::fvec3& value);
            void Set4f(std::string_view name, const Core::Math::fvec4& value);
            void SetMat3fv(std::string_view name, const Core::Math::mat3& value, GLboolean transpose = GL_FALSE);
            void SetMat4fv(std::string_view name, const Core::Math::mat4& value, GLboolean transpose = GL_FALSE);

        private:
            int GetUniformLocation(std::string_view name);

            GLuint m_handle;
            bool m_isLinked;
            const std::string m_vertex;
            const std::string m_fragment;
            std::unordered_map<std::string, int> m_uniformLocationCache;

            const AssetManagers::ShaderSourceManager& m_shaderSourceManager;
        };
    }
}