#version 330 core
layout (location = 0) in vec4 position;
layout (location = 1) in vec4 normal;
layout (location = 2) in vec2 texCoord;

out vec2 v_TexCoord;
  
uniform mat4 mvp;

void main()
{
    gl_Position = mvp * position;
    v_TexCoord = texCoord;
}