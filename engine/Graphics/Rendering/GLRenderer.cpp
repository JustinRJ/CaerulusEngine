#include "stdafx.h"

#include "Rendering/GLRenderer.h"

#include "Math/Math.h"
#include "Logging/Log.h"
#include "Window/GLWindow.h"
#include "Pipeline/VertexArray.h"
#include "Pipeline/VertexBuffer.h"
#include "Pipeline/IndexBuffer.h"
#include "Geometry/GPUGeometry.h"

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
        LogError("OpenGL Error " + std::string(message), "OpenGL Error Code-" + std::to_string(id));
    }
}

namespace Graphics
{
    namespace Rendering
    {
        GLRenderer::GLRenderer() :
            m_wireframe(false)
        {
            glewExperimental = true;

            GLenum error = glGetError();
            if (error != GL_NO_ERROR)
            {
                LogError("OpenGL Error", std::to_string(error));
                exit(1);
            }

            if (glewInit() != GLEW_OK)
            {
                LogError("Failed to init GLEW!");
                exit(1);
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);

            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(OpenGLErrorLogCallback, 0);
        }

        void GLRenderer::Clear(Core::Math::vec4 colour) const
        {
            glClearColor(colour.r, colour.g, colour.b, colour.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void GLRenderer::Draw(const Pipeline::VertexArray& va, const Pipeline::IndexBuffer& ib) const
        {
            va.Bind();
            ib.Bind();
            glDrawElements(m_wireframe ? GL_LINE_LOOP : GL_TRIANGLES, static_cast<GLsizei>(ib.GetCount()), GL_UNSIGNED_INT, nullptr);
        }

        void GLRenderer::Draw(const Geometry::GPUGeometry& geometry) const
        {
            Draw(geometry.GetVertexArray(), geometry.GetIndexBuffer());
        }

        void GLRenderer::DrawSphere(Core::Math::vec3 position, double radius, Core::Math::vec3 colour) const
        {
            // TODO
        }

        void GLRenderer::DrawRay(Core::Math::vec3 start, Core::Math::vec3 end, Core::Math::vec3 colour, float lineWidth) const
        {
            glLineWidth(lineWidth);
            glColor3f(colour.r, colour.g, colour.b);
            glBegin(GL_LINES);
                glVertex3f(start.x, start.y, start.z);
                glVertex3f(end.x, end.y, end.z);
                // TODO - draw direction
            glEnd();
        }

        void GLRenderer::DrawLine(Core::Math::vec3 point1, Core::Math::vec3 point2, Core::Math::vec3 colour, float lineWidth) const
        {
            glLineWidth(lineWidth);
            glColor3f(colour.r, colour.g, colour.b);
            glBegin(GL_LINES);
                glVertex3f(point1.x, point1.y, point1.z);
                glVertex3f(point2.x, point2.y, point2.z);
            glEnd();
        }


        void GLRenderer::SetWireframe(bool wireframe)
        {
            m_wireframe = wireframe;
        }

        bool GLRenderer::GetWireframe() const
        {
            return m_wireframe;
        }
    }
}
