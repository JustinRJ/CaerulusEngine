#include "stdafx.h"

#include "Shader.h"

#include "ShaderSrc.h"
#include "Core/Logging/Log.h"

namespace Graphics
{
    namespace Pipeline
    {
        Shader::Shader(std::shared_ptr<ShaderSrc> vertex, std::shared_ptr<ShaderSrc> fragment) :
            m_vertex(vertex),
            m_fragment(fragment)
        {
            using Core::Logging::Log;
            const unsigned int logSize = 512;
            // Shader Program Compilation
            GLchar infoLog[logSize];
            m_handle = glCreateProgram();
            glAttachShader(m_handle, m_vertex->GetHandle());
            glAttachShader(m_handle, m_fragment->GetHandle());
            glLinkProgram(m_handle);

            GLint isLinkSuccessful = false;
            glGetProgramiv(m_handle, GL_LINK_STATUS, &isLinkSuccessful);

            if (!isLinkSuccessful)
            {
                glGetProgramInfoLog(m_handle, logSize, NULL, infoLog);
                Log::LogError("Shader program linking failed!", infoLog);
            }

            m_isLinked = true;
        }

        void Shader::UpdateUniforms()
        {
            for (auto const& kv : m_uniformCallbackMap.IntCallbacks)
            {
                Set1i(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.FloatCallbacks)
            {
                Set1f(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.Vec2Callbacks)
            {
                Set2f(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.Vec3Callbacks)
            {
                Set3f(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.Vec4Callbacks)
            {
                Set4f(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.Mat3Callbacks)
            {
                SetMat3fv(kv.first, kv.second());
            }

            for (auto const& kv : m_uniformCallbackMap.Mat4Callbacks)
            {
                SetMat4fv(kv.first, kv.second());
            }
        }
    }
}