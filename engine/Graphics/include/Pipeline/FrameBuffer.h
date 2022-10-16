#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <string>

#include "Interface/NonCopyable.h"

namespace Graphics
{
    namespace Pipeline
    {
        class CAERULUS_GRAPHICS FrameBuffer : Core::Interface::NonCopyable
        {
        public:
            FrameBuffer() = default;
            ~FrameBuffer();

            bool Init(uint32_t width, uint32_t height, uint32_t msaa = 0);
            void Bind();
            static void Unbind();
            void Update();

            void BlitColorTo(GLuint fbo, int x = 0, int y = 0, int w = 0, int h = 0);
            void BlitDepthTo(GLuint fbo, int x = 0, int y = 0, int w = 0, int h = 0);

            void CopyColorBuffer();
            void CopyDepthBuffer();
            const unsigned char* GetColorBuffer() const;
            const float* GetDepthBuffer() const;

            GLuint GetID() const;
            GLuint GetColorID() const;
            GLuint GetDepthID() const;

            uint32_t GetWidth() const;
            uint32_t GetHeight() const;
            uint32_t GetMsaa() const;
            std::string_view GetErrorMessage() const;
            std::string GetStatus() const;

        private:

            void DeleteBuffers();
            bool CheckFrameBufferStatus();

            static std::string GetTextureParameters(GLuint id);
            static std::string GetRenderbufferParameters(GLuint id);
            static std::string ConvertInternalFormatToString(GLenum format);

            uint32_t m_width = 0;         // buffer width
            uint32_t m_height = 0;        // buffer height
            uint32_t m_msaa = 0;          // # of multi samples; 0, 2, 4, 8,...
            unsigned char* m_colorBuffer = nullptr;   // color buffer (rgba)
            float* m_depthBuffer = nullptr;           // depth buffer
            GLuint m_fboMsaaId = 0;           // primary id for multisample FBO
            GLuint m_rboMsaaColorId = 0;      // id for multisample RBO (color buffer)
            GLuint m_rboMsaaDepthId = 0;      // id for multisample RBO (depth buffer)
            GLuint m_fboId = 0;               // secondary id for frame buffer object
            GLuint m_texId = 0;               // id for texture object (color buffer)
            GLuint m_rboId = 0;               // id for render buffer object (depth buffer)

            std::string m_errorMessage = "";
        };
    }
}