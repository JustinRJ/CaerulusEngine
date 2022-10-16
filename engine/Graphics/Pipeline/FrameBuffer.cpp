#include "stdafx.h"

#include "Pipeline\FrameBuffer.h"

#include <sstream>

namespace Graphics
{
    namespace Pipeline
    {
        FrameBuffer::~FrameBuffer()
        {
            DeleteBuffers();
        }

        bool FrameBuffer::Init(uint32_t width, uint32_t height, uint32_t msaa)
        {
            int maxMsaa = 0;
            glGetIntegerv(GL_MAX_SAMPLES, &maxMsaa);
            if (msaa > static_cast<uint32_t>(maxMsaa))
            {
                msaa = maxMsaa;
            }
            else if (msaa % 2 != 0)
            {
                msaa--;
            }

            // reset error message
            m_errorMessage = "";

            m_width = width;
            m_height = height;
            m_msaa = msaa;

            // reset buffers
            DeleteBuffers();

            // create arrays
            m_depthBuffer = new float[m_width * m_height];                // 32 bits per pixel
            m_colorBuffer = new unsigned char[m_width * m_height * 4];    // 32 bits per pixel

            // create single-sample FBO
            glGenFramebuffers(1, &m_fboId);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);

            // create a texture object to store colour info, and attach it to fbo
            glGenTextures(1, &m_texId);
            glBindTexture(GL_TEXTURE_2D, m_texId);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texId, 0);

            // create a renderbuffer object to store depth info, attach it to fbo
            glGenRenderbuffers(1, &m_rboId);
            glBindRenderbuffer(GL_RENDERBUFFER, m_rboId);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboId);

            // check FBO completeness
            bool status = CheckFrameBufferStatus();

            // create multi-sample fbo
            if (msaa > 0)
            {
                glGenFramebuffers(1, &m_fboMsaaId);
                glBindFramebuffer(GL_FRAMEBUFFER, m_fboMsaaId);

                // create a render buffer object to store colour info
                glGenRenderbuffers(1, &m_rboMsaaColorId);
                glBindRenderbuffer(GL_RENDERBUFFER, m_rboMsaaColorId);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_msaa, GL_RGB8, m_width, m_height);

                // attach a renderbuffer to FBO color attachment point
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rboMsaaColorId);

                // create a renderbuffer object to store depth info
                glGenRenderbuffers(1, &m_rboMsaaDepthId);
                glBindRenderbuffer(GL_RENDERBUFFER, m_rboMsaaDepthId);
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_msaa, GL_DEPTH_COMPONENT, m_width, m_height);

                // attach a renderbuffer to FBO depth attachment point
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboMsaaDepthId);

                // check FBO completeness again
                status = CheckFrameBufferStatus();
            }

            // unbound
            glBindRenderbuffer(GL_RENDERBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return status;
        }

        void FrameBuffer::DeleteBuffers()
        {
            if (m_rboMsaaColorId)
            {
                glDeleteRenderbuffers(1, &m_rboMsaaColorId);
                m_rboMsaaColorId = 0;
            }
            if (m_rboMsaaDepthId)
            {
                glDeleteRenderbuffers(1, &m_rboMsaaDepthId);
                m_rboMsaaDepthId = 0;
            }
            if (m_fboMsaaId)
            {
                glDeleteFramebuffers(1, &m_fboMsaaId);
                m_fboMsaaId = 0;
            }
            if (m_texId)
            {
                glDeleteTextures(1, &m_texId);
                m_texId = 0;
            }
            if (m_rboId)
            {
                glDeleteRenderbuffers(1, &m_rboId);
                m_rboId = 0;
            }
            if (m_fboId)
            {
                glDeleteFramebuffers(1, &m_fboId);
                m_fboId = 0;
            }

            delete[] m_colorBuffer;
            m_colorBuffer = 0;
            delete[] m_depthBuffer;
            m_depthBuffer = 0;
        }

        GLuint FrameBuffer::GetID() const
        {
            return m_msaa == 0 ? m_fboId : m_fboMsaaId;
        }

        void FrameBuffer::Bind()
        {
            if (m_msaa == 0)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, m_fboId);
            }
            else
            {
                glBindFramebuffer(GL_FRAMEBUFFER, m_fboMsaaId);
            }
        }

        void FrameBuffer::Unbind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void FrameBuffer::Update()
        {
            if (m_msaa > 0)
            {
                // blit both color and depth
                glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboMsaaId);
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fboId);
                glBlitFramebuffer(0, 0, m_width, m_height,
                    0, 0, m_width, m_height,
                    GL_COLOR_BUFFER_BIT,
                    GL_LINEAR);
            }

            // also, generate mipmaps for color buffer (texture)
            glBindTexture(GL_TEXTURE_2D, m_texId);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void FrameBuffer::BlitColorTo(GLuint dstId, int x, int y, int width, int height)
        {
            // if width/height not specified, use src dimension
            if (width == 0)
            {
                width = m_width;
            }

            if (height == 0)
            {
                height = m_height;
            }

            GLuint srcId = (m_msaa == 0) ? m_fboId : m_fboMsaaId;
            glBindFramebuffer(GL_READ_FRAMEBUFFER, srcId);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstId);
            glBlitFramebuffer(0, 0, m_width, m_height,  // src rect
                0, 0, width, height,              // dst rect
                GL_COLOR_BUFFER_BIT,              // buffer mask
                GL_LINEAR);                       // scale filter
        }

        void FrameBuffer::BlitDepthTo(GLuint dstId, int x, int y, int width, int height)
        {
            // if width/height not specified, use src dimension
            if (width == 0)
            {
                width = m_width;
            }

            if (height == 0)
            {
                height = m_height;
            }

            // NOTE: scale filter for depth buffer must be GL_NEAREST, otherwise, invalid op
            GLuint srcId = (m_msaa == 0) ? m_fboId : m_fboMsaaId;
            glBindFramebuffer(GL_READ_FRAMEBUFFER, srcId);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstId);
            glBlitFramebuffer(0, 0, m_width, m_height,  // src rect
                0, 0, width, height,              // dst rect
                GL_DEPTH_BUFFER_BIT,              // buffer mask
                GL_NEAREST);                      // scale filter
        }

        void FrameBuffer::CopyColorBuffer()
        {
            BlitColorTo(m_fboId); // copy multi-sample to single-sample first
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboId);
            glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_colorBuffer);
        }

        void FrameBuffer::CopyDepthBuffer()
        {
            BlitDepthTo(m_fboId);  // copy multi-sample to single-sample first
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fboId);
            glReadPixels(0, 0, m_width, m_height, GL_DEPTH_COMPONENT, GL_FLOAT, m_depthBuffer);
        }

        bool FrameBuffer::CheckFrameBufferStatus()
        {
            // check FBO status
            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            switch (status)
            {
            case GL_FRAMEBUFFER_COMPLETE:
                m_errorMessage = "";
                return true;

            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                m_errorMessage = "[ERROR] Framebuffer incomplete: Attachment is NOT complete.";
                return false;

            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                m_errorMessage = "[ERROR] Framebuffer incomplete: No image is attached to FBO.";
                return false;
                /*
                    case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
                        errorMessage = "[ERROR] Framebuffer incomplete: Attached images have different dimensions.";
                        return false;

                    case GL_FRAMEBUFFER_INCOMPLETE_FORMATS:
                        errorMessage = "[ERROR] Framebuffer incomplete: Color attached images have different internal formats.";
                        return false;
                */
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                m_errorMessage = "[ERROR] Framebuffer incomplete: Draw buffer.";
                return false;

            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                m_errorMessage = "[ERROR] Framebuffer incomplete: Read buffer.";
                return false;

            case GL_FRAMEBUFFER_UNSUPPORTED:
                m_errorMessage = "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation.";
                return false;

            default:
                m_errorMessage = "[ERROR] Framebuffer incomplete: Unknown error.";
                return false;
            }
        }

        std::string FrameBuffer::GetStatus() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, m_msaa == 0 ? m_fboId : m_fboMsaaId);

            std::stringstream ss;

            ss << "\n===== FBO STATUS =====\n";

            // print max # of colorbuffers supported by FBO
            int colorBufferCount = 0;
            glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &colorBufferCount);
            ss << "Max Number of Color Buffer Attachment Points: " << colorBufferCount << std::endl;

            // get max # of multi samples
            int multiSampleCount = 0;
            glGetIntegerv(GL_MAX_SAMPLES, &multiSampleCount);
            ss << "Max Number of Samples for MSAA: " << multiSampleCount << std::endl;

            int objectType;
            int objectId;

            // print info of the colorbuffer attachable image
            for (int i = 0; i < colorBufferCount; ++i)
            {
                glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0 + i,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                    &objectType);

                if (objectType != GL_NONE)
                {
                    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                        GL_COLOR_ATTACHMENT0 + i,
                        GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                        &objectId);

                    std::string formatName;

                    ss << "Color Attachment " << i << ": ";
                    if (objectType == GL_TEXTURE)
                    {
                        ss << "GL_TEXTURE, " << FrameBuffer::GetTextureParameters(objectId) << std::endl;
                    }
                    else if (objectType == GL_RENDERBUFFER)
                    {
                        ss << "GL_RENDERBUFFER, " << FrameBuffer::GetRenderbufferParameters(objectId) << std::endl;
                    }
                }
            }

            // print info of the depthbuffer attachable image
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                GL_DEPTH_ATTACHMENT,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                &objectType);

            if (objectType != GL_NONE)
            {
                glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                    &objectId);

                ss << "Depth Attachment: ";
                switch (objectType)
                {
                case GL_TEXTURE:
                    ss << "GL_TEXTURE, " << FrameBuffer::GetTextureParameters(objectId) << std::endl;
                    break;
                case GL_RENDERBUFFER:
                    ss << "GL_RENDERBUFFER, " << FrameBuffer::GetRenderbufferParameters(objectId) << std::endl;
                    break;
                }
            }

            // print info of the stencilbuffer attachable image
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                GL_STENCIL_ATTACHMENT,
                GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE,
                &objectType);
            if (objectType != GL_NONE)
            {
                glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                    GL_STENCIL_ATTACHMENT,
                    GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                    &objectId);

                ss << "Stencil Attachment: ";
                switch (objectType)
                {
                case GL_TEXTURE:
                    ss << "GL_TEXTURE, " << FrameBuffer::GetTextureParameters(objectId) << std::endl;
                    break;
                case GL_RENDERBUFFER:
                    ss << "GL_RENDERBUFFER, " << FrameBuffer::GetRenderbufferParameters(objectId) << std::endl;
                    break;
                }
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return ss.str();
        }

        std::string FrameBuffer::GetTextureParameters(GLuint id)
        {
            if (glIsTexture(id) == GL_FALSE)
            {
                return "Not texture object";
            }

            int width, height, format;
            std::string formatName;
            glBindTexture(GL_TEXTURE_2D, id);
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);            // get texture width
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);          // get texture height
            glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format); // get texture internal format
            glBindTexture(GL_TEXTURE_2D, 0);

            formatName = FrameBuffer::ConvertInternalFormatToString(format);

            std::stringstream ss;
            ss << width << "x" << height << ", " << formatName;
            return ss.str();
        }

        std::string FrameBuffer::GetRenderbufferParameters(GLuint id)
        {
            if (glIsRenderbuffer(id) == GL_FALSE)
            {
                return "Not Renderbuffer object";
            }

            int width, height, format, samples;
            std::string formatName;
            glBindRenderbuffer(GL_RENDERBUFFER, id);
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);    // get renderbuffer width
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);  // get renderbuffer height
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &format); // get renderbuffer internal format
            glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_SAMPLES, &samples);   // get multisample count
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            formatName = FrameBuffer::ConvertInternalFormatToString(format);

            std::stringstream ss;
            ss << width << "x" << height << ", " << formatName << ", MSAA(" << samples << ")";
            return ss.str();
        }

        const unsigned char* FrameBuffer::GetColorBuffer() const
        {
            return m_colorBuffer;
        }

        const float* FrameBuffer::GetDepthBuffer() const
        {
            return m_depthBuffer;
        }

        GLuint FrameBuffer::GetColorID() const
        {
            return m_texId;
        }

        GLuint FrameBuffer::GetDepthID() const
        {
            return m_rboId;
        }

        uint32_t FrameBuffer::GetWidth() const
        {
            return m_width;
        }

        uint32_t FrameBuffer::GetHeight() const
        {
            return m_height;
        }

        uint32_t FrameBuffer::GetMsaa() const
        {
            return m_msaa;
        }

        std::string_view FrameBuffer::GetErrorMessage() const
        {
            return m_errorMessage;
        }

        std::string FrameBuffer::ConvertInternalFormatToString(GLenum format)
        {
            std::string formatName;

            switch (format)
            {
            case GL_STENCIL_INDEX:      // 0x1901
                formatName = "GL_STENCIL_INDEX";
                break;
            case GL_DEPTH_COMPONENT:    // 0x1902
                formatName = "GL_DEPTH_COMPONENT";
                break;
            case GL_ALPHA:              // 0x1906
                formatName = "GL_ALPHA";
                break;
            case GL_RG:                // 0x8227
                formatName = "GL_RG";  
                break;
            case GL_RGB:                // 0x1907
                formatName = "GL_RGB";
                break;
            case GL_RGBA:               // 0x1908
                formatName = "GL_RGBA";
                break;
            case GL_LUMINANCE:          // 0x1909
                formatName = "GL_LUMINANCE";
                break;
            case GL_LUMINANCE_ALPHA:    // 0x190A
                formatName = "GL_LUMINANCE_ALPHA";
                break;
            case GL_R3_G3_B2:           // 0x2A10
                formatName = "GL_R3_G3_B2";
                break;
            case GL_ALPHA4:             // 0x803B
                formatName = "GL_ALPHA4";
                break;
            case GL_ALPHA8:             // 0x803C
                formatName = "GL_ALPHA8";
                break;
            case GL_ALPHA12:            // 0x803D
                formatName = "GL_ALPHA12";
                break;
            case GL_ALPHA16:            // 0x803E
                formatName = "GL_ALPHA16";
                break;
            case GL_LUMINANCE4:         // 0x803F
                formatName = "GL_LUMINANCE4";
                break;
            case GL_LUMINANCE8:         // 0x8040
                formatName = "GL_LUMINANCE8";
                break;
            case GL_LUMINANCE12:        // 0x8041
                formatName = "GL_LUMINANCE12";
                break;
            case GL_LUMINANCE16:        // 0x8042
                formatName = "GL_LUMINANCE16";
                break;
            case GL_LUMINANCE4_ALPHA4:  // 0x8043
                formatName = "GL_LUMINANCE4_ALPHA4";
                break;
            case GL_LUMINANCE6_ALPHA2:  // 0x8044
                formatName = "GL_LUMINANCE6_ALPHA2";
                break;
            case GL_LUMINANCE8_ALPHA8:  // 0x8045
                formatName = "GL_LUMINANCE8_ALPHA8";
                break;
            case GL_LUMINANCE12_ALPHA4: // 0x8046
                formatName = "GL_LUMINANCE12_ALPHA4";
                break;
            case GL_LUMINANCE12_ALPHA12:// 0x8047
                formatName = "GL_LUMINANCE12_ALPHA12";
                break;
            case GL_LUMINANCE16_ALPHA16:// 0x8048
                formatName = "GL_LUMINANCE16_ALPHA16";
                break;
            case GL_INTENSITY:          // 0x8049
                formatName = "GL_INTENSITY";
                break;
            case GL_INTENSITY4:         // 0x804A
                formatName = "GL_INTENSITY4";
                break;
            case GL_INTENSITY8:         // 0x804B
                formatName = "GL_INTENSITY8";
                break;
            case GL_INTENSITY12:        // 0x804C
                formatName = "GL_INTENSITY12";
                break;
            case GL_INTENSITY16:        // 0x804D
                formatName = "GL_INTENSITY16";
                break;
            case GL_RG16F:              // 0x822F
                formatName = "GL_RG16F";
                break;
            case GL_RGB4:               // 0x804F
                formatName = "GL_RGB4";
                break;
            case GL_RGB5:               // 0x8050
                formatName = "GL_RGB5";
                break;
            case GL_RGB8:               // 0x8051
                formatName = "GL_RGB8";
                break;
            case GL_RGB10:              // 0x8052
                formatName = "GL_RGB10";
                break;
            case GL_RGB12:              // 0x8053
                formatName = "GL_RGB12";
                break;
            case GL_RGB16:              // 0x8054
                formatName = "GL_RGB16";
                break;
            case GL_RGBA2:              // 0x8055
                formatName = "GL_RGBA2";
                break;
            case GL_RGBA4:              // 0x8056
                formatName = "GL_RGBA4";
                break;
            case GL_RGB5_A1:            // 0x8057
                formatName = "GL_RGB5_A1";
                break;
            case GL_RGBA8:              // 0x8058
                formatName = "GL_RGBA8";
                break;
            case GL_RGB10_A2:           // 0x8059
                formatName = "GL_RGB10_A2";
                break;
            case GL_RGBA12:             // 0x805A
                formatName = "GL_RGBA12";
                break;
            case GL_RGBA16:             // 0x805B
                formatName = "GL_RGBA16";
                break;
            case GL_DEPTH_COMPONENT16:  // 0x81A5
                formatName = "GL_DEPTH_COMPONENT16";
                break;
            case GL_DEPTH_COMPONENT24:  // 0x81A6
                formatName = "GL_DEPTH_COMPONENT24";
                break;
            case GL_DEPTH_COMPONENT32:  // 0x81A7
                formatName = "GL_DEPTH_COMPONENT32";
                break;
            case GL_DEPTH_STENCIL:      // 0x84F9
                formatName = "GL_DEPTH_STENCIL";
                break;
            case GL_RGBA32F:            // 0x8814
                formatName = "GL_RGBA32F";
                break;
            case GL_RGB32F:             // 0x8815
                formatName = "GL_RGB32F";
                break;
            case GL_RGBA16F:            // 0x881A
                formatName = "GL_RGBA16F";
                break;
            case GL_RGB16F:             // 0x881B
                formatName = "GL_RGB16F";
                break;
            case GL_DEPTH24_STENCIL8:   // 0x88F0
                formatName = "GL_DEPTH24_STENCIL8";
                break;
            default:
                std::stringstream ss;
                ss << "Unknown Format(0x" << std::hex << format << ")" << std::ends;
                formatName = ss.str();
            }

            return formatName;
        }
    }
}
