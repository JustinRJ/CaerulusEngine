#include "stdafx.h"

#include "Renderer.h"

#include "Core/Math/Math.h"
#include "Core/Logging/Log.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Pipeline/Shader.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace
{
    void GLAPIENTRY OpenGLErrorLogCallback(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar* message,
            const void* userParam)
    {
        if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
        {
            using namespace Core::Logging;
            Log::LogError("OpenGL Error " + std::string(message), "OpenGL Error Code-" + std::to_string(id));
        }
    }
}

namespace Graphics
{
    namespace PipeLine
    {
        Renderer::Renderer()
        {
            glewExperimental = true;

            using namespace Core::Logging;
            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                Log::LogError("OpenGL Error", std::to_string(error));
                exit(1);
            }

            if (glewInit() != GLEW_OK)
            {
                Log::LogError("Failed to init GLEW!");
                exit(1);
            }

            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLErrorLogCallback, 0);
        }

        void Renderer::Clear()
        {
            glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, m_clearColour.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
        {
            shader.Bind();
            va.Bind();
            ib.Bind();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ib.GetCount()), GL_UNSIGNED_INT, nullptr);
        }

        void Renderer::SwapBuffer(float deltaTime)
        {
            m_window->SwapBuffer();
        }

        std::shared_ptr<Window::GLWindow> Renderer::GetWindow() const
        {
            return m_window;
        }

        std::shared_ptr<PipeLine::Shader> Renderer::GetShader() const
        {
            return m_shader;
        }

        std::shared_ptr<Core::Math::Camera> Renderer::GetCamera() const
        {
            return m_camera;
        }

        void Renderer::SetWindow(std::shared_ptr<Window::GLWindow> window)
        {
            m_window = window;
        }

        void Renderer::SetShader(std::shared_ptr<PipeLine::Shader> shader)
        {
            m_shader = shader;
        }

        void Renderer::SetCamera(std::shared_ptr<Core::Math::Camera> camera)
        {
            m_camera = camera;
        }

        void Renderer::SetClearColour(glm::vec4 colour)
        {
            m_clearColour = colour;
        }

        glm::vec4 Renderer::GetClearColour() const
        {
            return m_clearColour;
        }
    }
}
