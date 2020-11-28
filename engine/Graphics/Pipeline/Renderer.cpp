#include "stdafx.h"

#include "Renderer.h"

#include "Core/Math/Math.h"
#include "Core/Logging/Log.h"
#include "Graphics/Window/GLWindow.h"
#include "Graphics/Pipeline/VertexArray.h"
#include "Graphics/Pipeline/VertexBuffer.h"
#include "Graphics/Pipeline/IndexBuffer.h"

namespace
{
    using namespace Core::Logging;

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
            Log::LogError("OpenGL Error " + std::string(message), "OpenGL Error Code-" + std::to_string(id));
        }
    }
}

namespace Graphics
{
    namespace Pipeline
    {
        Renderer::Renderer()
        {
            glewExperimental = true;

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

        void Renderer::Clear(Core::Math::vec4 colour)
        {
            glClearColor(colour.r, colour.g, colour.b, colour.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, bool wireframe)
        {
            va.Bind();
            ib.Bind();
            glDrawElements(wireframe ? GL_LINE_LOOP : GL_TRIANGLES, static_cast<GLsizei>(ib.GetCount()), GL_UNSIGNED_INT, nullptr);
        }
    }
}
