#version 400 core

in vec2 TexCoords;
out float saoBlurOutput;

uniform sampler2D saoInput;
uniform int saoBlurSize;


void main()
{
   vec2 texelSize = 1.0f / vec2(textureSize(saoInput, 0));
   float result = 0.0f;

   for (int x = 0; x < saoBlurSize; ++x)
   {
      for (int y = 0; y < saoBlurSize; ++y)
      {
         vec2 offset = (vec2(-2.0f) + vec2(float(x), float(y))) * texelSize;
         result += texture(saoInput, TexCoords + offset).r;
      }
   }

   saoBlurOutput = result / float(saoBlurSize * saoBlurSize);
}
