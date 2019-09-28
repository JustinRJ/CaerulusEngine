#pragma once

#define CAERULUS_GRAPHICS __declspec(dllexport)

#include <glew.h>
#include <string>

namespace Graphics
{
    namespace Resource
    {
        enum TextureType
        {
            Default = 0,
            HDR = 1,
            CustomHDR = 2,
            Cube = 3,
        };

        class CAERULUS_GRAPHICS Texture
        {
        public:
            Texture();
            ~Texture();

            GLint GetID();

            bool Load(const char* texPath, bool texFlip);
            bool LoadHDR(const char* texPath, bool texFlip);
            void CreateHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);
            void CreateCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);

            GLuint GetWidth() const;
            GLuint GetHeight() const;

            const std::string& GetPath() const;
            void UseTexture() const;
           void ComputeMipmap();

        private:
            std::string& m_Path;
            GLfloat m_AnisoFilterLevel;
            GLuint m_ID;
            GLuint m_Width;
            GLuint m_Height;
            GLuint m_Components;

            GLenum m_Type;
            GLenum m_InternalFormat;
            GLenum m_Format;
        };
    }
}
