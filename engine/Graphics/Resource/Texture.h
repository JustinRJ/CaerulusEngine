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

        class Texture
        {
        public:
            CAERULUS_GRAPHICS Texture();
            CAERULUS_GRAPHICS ~Texture();

            CAERULUS_GRAPHICS GLint GetID();

            CAERULUS_GRAPHICS bool Load(const char* texPath, bool texFlip);
            CAERULUS_GRAPHICS bool LoadHDR(const char* texPath, bool texFlip);
            CAERULUS_GRAPHICS void CreateHDR(GLuint width, GLuint height, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);
            CAERULUS_GRAPHICS void CreateCube(GLuint width, GLenum format, GLenum internalFormat, GLenum type, GLenum minFilter);

            CAERULUS_GRAPHICS GLuint GetWidth() const;
            CAERULUS_GRAPHICS GLuint GetHeight() const;

            CAERULUS_GRAPHICS const std::string& GetPath() const;
            CAERULUS_GRAPHICS void UseTexture() const;
            CAERULUS_GRAPHICS void ComputeMipmap();

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
