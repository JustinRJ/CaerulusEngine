#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <string>
#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace PipeLine
    {
        class CAERULUS_GRAPHICS PiplineVertex : public Core::Interface::NonCopyable
        {
        public:
            PiplineVertex() = default;
            virtual ~PiplineVertex() = default;

            void Init()
            {
                glGenVertexArrays(1, &m_VAO);
                glGenBuffers(1, &m_VBO);
                glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            };

            void Bind() const
            {
                glBindVertexArray(m_VAO);
                glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            }

            void Unbind() const
            {
                glBindVertexArray(0);
            }

            GLuint GetVAO() const
            {
                return m_VAO;
            }

            GLuint GetVBO() const
            {
                return m_VBO;
            }

        protected:
            GLuint m_VAO = 0;
            GLuint m_VBO = 0;

        private:
            virtual void SetVertices() = 0;
        };
    }
}