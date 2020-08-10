#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include "Core/Interface/NonCopyable.h"

namespace Graphics
{
    namespace Resource
    {
        // TODO - break texture out into different impl. for each load function
        class CAERULUS_GRAPHICS Texture : Core::Interface::NonCopyable
        {
        public:
            Texture() = default;
            virtual ~Texture();

            void Bind() const;
            GLint GetHandle();

            bool Load(const char* texPath, bool texFlip);
            bool LoadHDR(const char* texPath, bool texFlip);
            void CreateHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);
            void CreateCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);

            void ComputeMipmap();

            GLuint GetWidth() const;
            GLuint GetHeight() const;

            const std::string& GetPath() const;

        private:
            std::string m_path;
            GLfloat m_anisoFilterLevel;
            GLuint m_handle;
            GLuint m_width;
            GLuint m_height;
            GLuint m_components;

            GLenum m_type;
            GLenum m_internalFormat;
            GLenum m_format;
        };
    }
}
